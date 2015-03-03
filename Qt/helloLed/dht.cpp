#include "dht.h"

DHT::DHT(QObject *parent) :
    QObject(parent)
{
    pTimer = new QTimer(this);
    QObject::connect(pTimer, SIGNAL(timeout()), this, SLOT(startThread()));
    outPin = 5;
    model = DHT11;
}

void DHT::setPin(int mOutPin, DHT_MODEL_t mModel)
{
    outPin = mOutPin;
    model = mModel;

    if (mModel == AUTO_DETECT)
    {
        model = DHT22;
        if (error == ERROR_TIMEOUT)
        {
            model = DHT11;
            // Warning: in case we auto detect a DHT11, you should wait at least 1000 msec
            // before your first read request. Otherwise you will get a time out error.
        }
    }
}

double DHT::getHumidity()
{
    readSensor();
    return humidity;
}

double DHT::getTemperature()
{
    readSensor();
    return temperature;
}

QString DHT::getStatusString()
{
    QString errStr = "OK";
    switch (error)
    {
    case ERROR_TIMEOUT:
        errStr = "TIMEOUT";
        break;

    case ERROR_CHECKSUM:
        errStr = "CHECKSUM";
        break;

    default:
        errStr = "OK";
        break;
    }
    return errStr;
}

void DHT::readSensor()
{
    // Make sure we don't poll the sensor too often
    // - Max sample rate DHT11 is 1 Hz   (duty cicle 1000 ms)
    // - Max sample rate DHT22 is 0.5 Hz (duty cicle 2000 ms)
    unsigned long startTime = millis();
    if ((unsigned long)(startTime - lastReadTime) < (model == DHT11 ? 999L : 1999L))
    {
        return;
    }
    lastReadTime = startTime;

    temperature = 0;
    humidity = 0;

    // Request sample
    digitalWrite(outPin, LOW); // Send start signal
    pinMode(outPin, OUTPUT);
    if (model == DHT11)
    {
        delay(18);
    }
    else
    {
        // This will fail for a DHT11 - that's how we can detect such a device
        delayMicroseconds(800);
    }

    pinMode(outPin, INPUT);
    digitalWrite(outPin, HIGH); // Switch bus to receive data

    // We're going to read 83 edges:
    // - First a FALLING, RISING, and FALLING edge for the start bit
    // - Then 40 bits: RISING and then a FALLING edge per bit
    // To keep our code simple, we accept any HIGH or LOW reading if it's max 85 usecs long
    word rawHumidity = 0;
    word rawTemperature = 0;
    word data = 0;

    for (int8_t i=-3; i<2*40; i++)
    {
        byte age;
        startTime = micros();

        do {
            age = (unsigned long)(micros() - startTime);
            if (age > 90)
            {
                error = ERROR_TIMEOUT;
                return;
            }
        }
        while(digitalRead(outPin) == (i & 1) ? HIGH : LOW);

        if (i >= 0 && (i & 1))
        {
            // Now we are being fed our 40 bits
            data <<= 1;

            // A zero max 30 usecs, a one at least 68 usecs.
            if (age > 30)
            {
                data |= 1; // we got a one
            }
        }

        switch (i)
        {
        case 31:
            rawHumidity = data;
            break;
        case 63:
            rawTemperature = data;
            data = 0;
            break;
        }
    }

    // Verify checksum

    if ((byte)(((byte)rawHumidity) + (rawHumidity >> 8) + ((byte)rawTemperature) + (rawTemperature >> 8)) != data)
    {
        error = ERROR_CHECKSUM;
        return;
    }

    // Store readings
    if (model == DHT11)
    {
        humidity = rawHumidity >> 8;
        temperature = rawTemperature >> 8;
    }
    else
    {
        humidity = rawHumidity * 0.1;

        if (rawTemperature & 0x8000)
        {
            rawTemperature = -(int16_t)(rawTemperature & 0x7FFF);
        }
        temperature = ((int16_t)rawTemperature) * 0.1;
        qDebug() << rawHumidity << rawTemperature << error << 123;
    }

    error = ERROR_NONE;
}

void DHT::startThread()
{
    humidity = 0;
    temperature = 0;
    error = ERROR_NONE;
    lastReadTime = millis() - 3000;

    readSensor();

    qDebug() << humidity << temperature << error;

    if(humidity != 0)
    {
        emit sendHumidity(humidity);
    }
    if(temperature != 0)
    {
        emit sendTemperature(temperature);
    }
    emit sendError(error);

    pTimer->start(this->getMinimumSamplingPeriod());
}

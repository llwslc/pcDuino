#ifndef DHT_H
#define DHT_H

#include "common.h"

typedef enum {
    AUTO_DETECT,
    DHT11,
    DHT22,
    AM2302,  // Packaged DHT22
    RHT03    // Equivalent to DHT22
}
DHT_MODEL_t;

typedef enum {
    ERROR_NONE = 0,
    ERROR_TIMEOUT,
    ERROR_CHECKSUM
}
DHT_ERROR_t;

class DHT : public QObject
{
    Q_OBJECT
public:
    explicit DHT(QObject *parent = 0);

    DHT_MODEL_t model;
    DHT_ERROR_t error;

    int outPin;
    QTimer *pTimer;

    double temperature;
    double humidity;
    unsigned long lastReadTime;

    void setPin(int mOutPin, DHT_MODEL_t model = AUTO_DETECT);
    void readSensor();

    double getTemperature();
    double getHumidity();

    DHT_ERROR_t getStatus() { return error; }
    QString getStatusString();
    DHT_MODEL_t getModel() { return model; }

    int getMinimumSamplingPeriod() { return model == DHT11 ? 1000 : 2000; }

    int8_t getNumberOfDecimalsTemperature() { return model == DHT11 ? 0 : 1; }
    int8_t getLowerBoundTemperature() { return model == DHT11 ? 0 : -40; }
    int8_t getUpperBoundTemperature() { return model == DHT11 ? 50 : 125; }

    int8_t getNumberOfDecimalsHumidity() { return 0; }
    int8_t getLowerBoundHumidity() { return model == DHT11 ? 20 : 0; }
    int8_t getUpperBoundHumidity() { return model == DHT11 ? 90 : 100; }

    static float toFahrenheit(float fromCelcius) { return 1.8 * fromCelcius + 32.0; }
    static float toCelsius(float fromFahrenheit) { return (fromFahrenheit - 32.0) / 1.8; }

signals:
    void sendTemperature(double i);
    void sendHumidity(double i);
    void sendError(int i);

public slots:
    void startThread();
    
};

#endif // DHT_H

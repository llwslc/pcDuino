#include "infrared.h"

infrared::infrared(QObject *parent) :
    QObject(parent)
{
    pTimer = new QTimer(this);
    QObject::connect(pTimer, SIGNAL(timeout()), this, SLOT(startThread()));
    outPin = 0;
}

void infrared::setPin(int mOutPin)
{
    outPin = mOutPin;
}

void infrared::startThread()
{
    int status = digitalRead(outPin);
    delay(50);
    emit sendData(status);

    pTimer->start(1000);
}

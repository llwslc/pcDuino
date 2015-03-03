#include "pwmled.h"

PWMLed::PWMLed(QObject *parent) :
    QObject(parent)
{
    pTimer = new QTimer(this);
    QObject::connect(pTimer, SIGNAL(timeout()), this, SLOT(startThread()));
    outPin = 10;
}

void PWMLed::setPin(int mOutPin)
{
    outPin = mOutPin;
}

void PWMLed::startThread()
{
    int i;
    //亮度逐渐增加
    for(i=0; i<=0x15; i++)
    {
        analogWrite(outPin, i); //设置PWM占空比
        delay(50);
    }
    //亮度逐渐衰减
    for(i=0x15; i>=0; i--)
    {
        analogWrite(outPin, i); //设置PWM占空比
        delay(50);
    }

    pTimer->start(1000);
}

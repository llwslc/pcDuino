#ifndef PWMLED_H
#define PWMLED_H

#include "common.h"

class PWMLed : public QObject
{
    Q_OBJECT
public:
    explicit PWMLed(QObject *parent = 0);

    int outPin;
    QTimer *pTimer;

    void setPin(int mOutPin);
    
signals:
    void sendData(int i);
    
public slots:
    void startThread();
    
};

#endif // PWMLED_H


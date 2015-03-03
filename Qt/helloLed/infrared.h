#ifndef INFRARED_H
#define INFRARED_H

#include "common.h"

// 红外避障
class infrared : public QObject
{
    Q_OBJECT
public:
    explicit infrared(QObject *parent = 0);

    int outPin;
    QTimer *pTimer;

    void setPin(int mOutPin);

signals:
    void sendData(int i);

public slots:
    void startThread();
    
};

#endif // INFRARED_H

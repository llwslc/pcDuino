#ifndef HELLOLED_H
#define HELLOLED_H

#include "common.h"
#include "pwmled.h"
#include "infrared.h"
#include "dht.h"

namespace Ui {
class helloLed;
}

class helloLed : public QDialog
{
    Q_OBJECT
    
public:
    explicit helloLed(QWidget *parent = 0);
    ~helloLed();

    int ledPin;
    PWMLed *pPWMLedObject;
    QThread *pPWMLedThread;

    int infPin;
    infrared *pInfraredObject;
    QThread *pInfraredThread;

    int DHTPin;
    DHT *pDHTObject;
    QThread *pDHTThread;
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void infraredStatus(int i);

    void on_pushButton_7_clicked();

    void on_pushButton_6_clicked();

    void DHTTemperature(double i);

    void DHTHumidity(double i);

    void DHTError(int i);

private:
    Ui::helloLed *ui;
};

#endif // HELLOLED_H

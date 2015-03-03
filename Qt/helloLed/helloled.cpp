#include "helloled.h"
#include "ui_helloled.h"

helloLed::helloLed(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::helloLed)
{
    ui->setupUi(this);

    QRegExp regExp("^[0-9]+$");
    QRegExpValidator *pRevalidator = new QRegExpValidator(regExp, 0);
    ui->lineEdit->setValidator(pRevalidator);
    ui->lineEdit->setText("10");
    ledPin = ui->lineEdit->text().trimmed().toInt();

    pPWMLedObject = new PWMLed;
    pPWMLedThread = new QThread;
    pPWMLedObject->moveToThread(pPWMLedThread);
    QObject::connect(pPWMLedThread, SIGNAL(started()), pPWMLedObject, SLOT(startThread()));

    pinMode(ledPin, OUTPUT);

    ui->lineEdit_2->setValidator(pRevalidator);
    ui->lineEdit_2->setText("0");
    infPin = ui->lineEdit_2->text().trimmed().toInt();

    pInfraredObject = new infrared;
    pInfraredThread = new QThread;
    pInfraredObject->moveToThread(pInfraredThread);
    QObject::connect(pInfraredThread, SIGNAL(started()), pInfraredObject, SLOT(startThread()));
    QObject::connect(pInfraredObject, SIGNAL(sendData(int)), this, SLOT(infraredStatus(int)));

    pinMode(infPin, INPUT);

    ui->lineEdit_3->setValidator(pRevalidator);
    ui->lineEdit_3->setText("5");
    DHTPin = ui->lineEdit_3->text().trimmed().toInt();
    ui->comboBox->clear();
    ui->comboBox->addItem("DHT11");
    ui->comboBox->addItem("DHT22");
    ui->comboBox->setCurrentIndex(0);

    pDHTObject = new DHT;
    pDHTThread = new QThread;
    pDHTObject->moveToThread(pDHTThread);
    QObject::connect(pDHTThread, SIGNAL(started()), pDHTObject, SLOT(startThread()));
    QObject::connect(pDHTObject, SIGNAL(sendTemperature(double)), this, SLOT(DHTTemperature(double)));
    QObject::connect(pDHTObject, SIGNAL(sendHumidity(double)), this, SLOT(DHTHumidity(double)));
    QObject::connect(pDHTObject, SIGNAL(sendError(int)), this, SLOT(DHTError(int)));

    pinMode(DHTPin, INPUT);
}

helloLed::~helloLed()
{
    pPWMLedThread->quit();
    pInfraredThread->quit();
    pDHTThread->quit();
    delete ui;
}

void helloLed::on_pushButton_clicked()
{
    //LED 开
    ledPin = ui->lineEdit->text().trimmed().toInt();
    digitalWrite(ledPin, HIGH); // turn on LED
    delay(100);
}

void helloLed::on_pushButton_2_clicked()
{
    //LED 关
    pPWMLedThread->quit();

    ledPin = ui->lineEdit->text().trimmed().toInt();
    digitalWrite(ledPin, LOW); // turn off LED
    delay(100);
}

void helloLed::on_pushButton_3_clicked()
{
    //LED PWM
    ledPin = ui->lineEdit->text().trimmed().toInt();
    pPWMLedObject->setPin(ledPin);
    pPWMLedThread->start();
}

void helloLed::on_pushButton_4_clicked()
{
    //红外监测 开
    infPin = ui->lineEdit_2->text().trimmed().toInt();
    pInfraredObject->setPin(infPin);
    pInfraredThread->start();
}

void helloLed::on_pushButton_5_clicked()
{
    //红外监测 关
    pInfraredThread->quit();
}

void helloLed::infraredStatus(int i)
{
    //红外状态
    ui->checkBox->setChecked(!i);
    switch(i)
    {
    case HIGH:
        ui->checkBox->setText(QString("远"));
        on_pushButton_2_clicked();
        break;
    case LOW:
        on_pushButton_clicked();
        ui->checkBox->setText(QString("近"));
        break;
    default:
        ui->checkBox->setText(QString("远"));
        break;
    }
}

void helloLed::on_pushButton_7_clicked()
{
    // 温湿度 开
    DHTPin = ui->lineEdit_3->text().trimmed().toInt();
    QString DHTModelStr = ui->comboBox->currentText();
    qDebug() << DHTModelStr;
    if(DHTModelStr == "DHT11")
    {
        pDHTObject->setPin(DHTPin, DHT11);
    }
    else
    {
        pDHTObject->setPin(DHTPin, DHT22);
    }
    pDHTThread->start();
}

void helloLed::on_pushButton_6_clicked()
{
    // 温湿度 关
    pDHTThread->quit();
}

void helloLed::DHTTemperature(double i)
{
    // 温度
    ui->lcdNumber->display(i);
}

void helloLed::DHTHumidity(double i)
{
    // 湿度
    ui->lcdNumber_2->display(i);
}

void helloLed::DHTError(int i)
{
    // 状态
    ui->label_7->setText("状态 : " + QString::number(i, 10));
}

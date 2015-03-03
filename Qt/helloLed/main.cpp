#include <QtGui/QApplication>
#include "helloled.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

    arduinoInit();

    helloLed w;
    w.show();
    
    return a.exec();
}

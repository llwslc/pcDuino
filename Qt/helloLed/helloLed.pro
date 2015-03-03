#-------------------------------------------------
#
# Project created by QtCreator 2015-02-02T02:20:01
#
#-------------------------------------------------

QT       += core gui

TARGET = helloLed
TEMPLATE = app


SOURCES += main.cpp\
        helloled.cpp \
    arduino/WMath.cpp \
    arduino/wiring_shift.c \
    arduino/wiring_pulse.c \
    arduino/wiring_digital.c \
    arduino/wiring_analog.c \
    arduino/wiring.c \
    arduino/WInterrupts.c \
    arduino/platform.c \
    pwmled.cpp \
    infrared.cpp \
    dht.cpp

HEADERS  += helloled.h \
    arduino/wiring_private.h \
    arduino/pins_arduino.h \
    arduino/binary.h \
    arduino/Arduino.h \
    pwmled.h \
    common.h \
    infrared.h \
    dht.h

FORMS    += helloled.ui

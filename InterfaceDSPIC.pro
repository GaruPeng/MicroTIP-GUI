#-------------------------------------------------
#
# Project created by QtCreator 2016-10-26T13:42:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = InterfaceDSPIC
TEMPLATE = app


SOURCES += main.cpp\
        InterfaceDSPIC.cpp \
    SerialCommunication.cpp \
    Dac.cpp

HEADERS  += InterfaceDSPIC.h \
    SerialCommunication.h \
    Dac.h

FORMS    += InterfaceDSPIC.ui

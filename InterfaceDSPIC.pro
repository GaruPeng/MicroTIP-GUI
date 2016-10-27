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
    SerialCommunication.cpp

HEADERS  += InterfaceDSPIC.h \
    SerialCommunication.h

FORMS    += InterfaceDSPIC.ui

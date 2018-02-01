#-------------------------------------------------
#
# Project created by QtCreator 2016-10-26T13:42:43
#
#-------------------------------------------------

QT       += core gui
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = InterfaceDSPIC
TEMPLATE = app


SOURCES += \
        main.cpp\
        InterfaceDSPIC.cpp \
    SerialCommunication.cpp \
    Dac.cpp \
    Multiplexer.cpp \
    haptic.cpp \
    scope.cpp

HEADERS  += \
    InterfaceDSPIC.h \
    SerialCommunication.h \
    Dac.h \
    Multiplexer.h \
    VirtuoseAPI.h \
    haptic.h \
    scope.h

FORMS    += InterfaceDSPIC.ui

DISTFILES += \
    virtuoseAPI.dll \
    virtuoseDLL.lib

win32: LIBS += -L$$PWD/./ -lvirtuoseDLL

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

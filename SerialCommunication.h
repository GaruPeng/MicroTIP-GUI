#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include <QStringList>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QThread>
#include <QList>

class SerialCommunication
{
public:
    SerialCommunication(QString port, qint32 baudrate);
    ~SerialCommunication();
    static QStringList getSerialNames();
    static QStringList getBaudRates();
private:
    void configure(qint32 baudrate, QString portName);
    void open();
    void close();
public slots:

signals:
    void threadStarted();
    void threadDone();
private:
    QSerialPort serialPort;
};

#endif // SERIALCOMMUNICATION_H

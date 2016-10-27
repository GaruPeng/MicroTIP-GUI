#include "SerialCommunication.h"

SerialCommunication::SerialCommunication()
{
}

SerialCommunication::SerialCommunication(QString portName, qint32 baudRate)
{
    configure(baudRate, portName);
    open();
}

SerialCommunication::~SerialCommunication()
{
    close();
}

QStringList SerialCommunication::getSerialNames()
{
    QStringList serialNames;
    QList<QSerialPortInfo> portInfos = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo portInfo : portInfos)
    {
       serialNames << portInfo.portName();
    }
    return serialNames;
}

QStringList SerialCommunication::getBaudRates()
{
    QStringList baudRates;

    baudRates << "110";
    baudRates << "150";
    baudRates << "300";
    baudRates << "600";
    baudRates << "1200";
    baudRates << "2400";
    baudRates << "4800";
    baudRates << "9600";
    baudRates << "19200";;
    baudRates << "38400";
    baudRates << "57600";
    baudRates << "115200";
    baudRates << "230400";
    baudRates << "460800";
    baudRates << "921600";

    return baudRates;
}

void SerialCommunication::configure(qint32 baudrate, QString portName)
{
    serialPort.setBaudRate(baudrate);
    serialPort.setPortName(portName);
}

int SerialCommunication::open()
{
    int error;

    serialPort.open(QIODevice::ReadWrite);
    error = serialPort.error();

    return error;
}

int SerialCommunication::close()
{
    int error;

    serialPort.close();
    error = serialPort.error();

    return error;
}

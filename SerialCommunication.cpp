#include "SerialCommunication.h"

SerialCommunication::SerialCommunication(QString portName, qint32 baudRate)
{
    this->configure(baudRate, portName);
    this->open();
}

SerialCommunication::~SerialCommunication()
{
    this->close();
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
    baudRates << "19200";
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

void SerialCommunication::open()
{
    if((!serialPort.isOpen()))
    {
        serialPort.open(QIODevice::ReadWrite);
    }
}

void SerialCommunication::close()
{
    if(serialPort.isOpen())
    {
        serialPort.close();
    }
}

void SerialCommunication::sendMessage(QByteArray messageToSend)
{
    if((serialPort.isOpen()==true) &&
       (!messageToSend.isEmpty()))
    {
        serialPort.write(messageToSend);
    }

    this->configureReception();
}

QByteArray SerialCommunication::receiveMessage()
{
    QByteArray message;
    while(serialPort.waitForReadyRead(10))
    {
        message.append(QString(serialPort.readAll()));
    }
    return message;
}

void SerialCommunication::configureReception()
{
    if(!receptionThread.isRunning())
    {
        QObject::connect(&receptionThread, SIGNAL(started()), this, SLOT(receive()));
        receptionThread.start();
        qDebug() << "Reception Thread Started" ;
        this->moveToThread(&receptionThread);
    }
}

void SerialCommunication::receive()
{

    while(1)
    {
        /* Append new data to messageBuffer */
        if(serialPort.bytesAvailable() > 0)
        {
            messageBuffer.append(serialPort.readAll().toHex());

        }

        if(messageBuffer.size())
        {
            QString str = QString(messageBuffer.left(2));
            bool isOk = false;
            int taille = str.toInt(&isOk,16);
            if(isOk && messageBuffer.size() == 2*taille)
            {
                emit workDone(messageBuffer.left(2*taille));
                qDebug() << "work done";
                messageBuffer.remove(0,2*taille);
            }
        }
        thread()->msleep(10);
    }
}

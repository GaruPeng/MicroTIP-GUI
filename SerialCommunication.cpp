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
    if(!serialPort.isOpen())
    {
        serialPort.open(QIODevice::ReadWrite);
        qDebug() << "Serial port opened";
    }
}

void SerialCommunication::close()
{
    if(receptionThread.isRunning())
    {
        keepReceivingMessage = false;
        receptionThread.quit();
        receptionThread.wait();
        qDebug() << "Reception thread stopped";
    }
    if(serialPort.isOpen())
    {
        serialPort.close();
        qDebug() << "Serial port closed";
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
        qDebug() << "Reception thread started" ;
        keepReceivingMessage = true;
        this->moveToThread(&receptionThread);
    }
}

void SerialCommunication::receive()
{
    bool processingMessage = false;
    int messageSize = 0;
    while(keepReceivingMessage)
    {
        /* Append new data to messageBuffer */
        if(serialPort.bytesAvailable() > 0)
        {
            messageBuffer.append(serialPort.readAll().toHex());
        }

        if(messageBuffer.size())
        {
            /* Compute message size */
            if(!processingMessage)
            {
                messageSize = 2 * QString(messageBuffer.left(2)).toInt(nullptr, 16);
                if(messageSize>0)
                {
                    processingMessage = true;
                }
                else /* messageSize = 0 cannot happen */
                {
                    messageBuffer.remove(0,2);
                    qDebug() << "Wrong message size";
                }
            }
            else if(messageBuffer.size() >= messageSize)
            {
                emit workDone(messageBuffer.left(messageSize));
                qDebug() << "New message available: 0x" + messageBuffer.left(messageSize);
                messageBuffer.remove(0,messageSize);
                processingMessage = false;
            }

            /* Notify reception of new message */

        }

        thread()->msleep(10);
    }
}

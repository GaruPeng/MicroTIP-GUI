#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include <QStringList>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QThread>
#include <QList>
#include <QDebug>

class SerialCommunication : public QObject
{
    Q_OBJECT
public:
    SerialCommunication(QString port, qint32 baudrate);
    ~SerialCommunication();
    static QStringList getSerialNames();
    static QStringList getBaudRates();
    void sendMessage(QByteArray messageToSend);
    QByteArray receiveMessage();
private:
    void configure(qint32 baudrate, QString portName);
    void open();
    void close();
    void configureReception();
    QSerialPort serialPort;
    QThread receptionThread;
    QByteArray messageBuffer;
public slots:
    void receive();
signals:
    void workDone(QByteArray);
};

#endif // SERIALCOMMUNICATION_H

#include "InterfaceDSPIC.h"
#include "ui_InterfaceDSPIC.h"

InterfaceDSPIC::InterfaceDSPIC(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InterfaceDSPIC)
{
    ui->setupUi(this);
    init();
    updateSerialPorts();
    updateBaudrates();
}

InterfaceDSPIC::~InterfaceDSPIC()
{
    delete ui;
    delete serial;
}

void InterfaceDSPIC::init()
{
    /* Disable Close Communication on startup */
    ui->btnCloseCommunicationWithMicrocontroller->setEnabled(false);

    /* Disable Emission/Reception groupbox on startup */
    ui->leMessageToSend->setEnabled(false);
    ui->btnSendMessage->setEnabled(false);
}

void InterfaceDSPIC::updateSerialPorts()
{
    ui->cmbSerialNames->clear();
    ui->cmbSerialNames->addItems(SerialCommunication::getSerialNames());
}

void InterfaceDSPIC::updateBaudrates()
{
    ui->cmbBaudRates->clear();
    ui->cmbBaudRates->addItems(SerialCommunication::getBaudRates());
    ui->cmbBaudRates->setCurrentIndex(7);
}

void InterfaceDSPIC::on_btnRefreshSerialNames_clicked()
{
    InterfaceDSPIC::updateSerialPorts();
    InterfaceDSPIC::updateBaudrates();
}

void InterfaceDSPIC::on_btnOpenCommunicationWithMicrocontroller_clicked()
{
    serial = new SerialCommunication(ui->cmbSerialNames->currentText(),
                                     ui->cmbBaudRates->currentText().toInt());

    ui->btnOpenCommunicationWithMicrocontroller->setEnabled(false);
    ui->btnCloseCommunicationWithMicrocontroller->setEnabled(true);
    ui->btnRefreshSerialNames->setEnabled(false);
    ui->cmbSerialNames->setEnabled(false);
    ui->cmbBaudRates->setEnabled(false);

    ui->btnSendMessage->setEnabled(true);
    ui->leMessageToSend->setEnabled(true);

    connect(serial, SIGNAL(workDone(QByteArray)), this, SLOT(on_newMessage(QByteArray)));
}

void InterfaceDSPIC::on_btnCloseCommunicationWithMicrocontroller_clicked()
{
    delete serial;
    ui->btnOpenCommunicationWithMicrocontroller->setEnabled(true);
    ui->btnCloseCommunicationWithMicrocontroller->setEnabled(false);
    ui->btnRefreshSerialNames->setEnabled(true);
    ui->cmbSerialNames->setEnabled(true);
    ui->cmbBaudRates->setEnabled(true);

    ui->btnSendMessage->setEnabled(false);
    ui->leMessageToSend->setEnabled(false);

    ui->leMessageToSend->clear();
}

void InterfaceDSPIC::on_btnSendMessage_clicked()
{
    ui->teConsole->append(QString("Sent : " + ui->leMessageToSend->text()));
    serial->sendMessage(QByteArray::fromHex(ui->leMessageToSend->text().toLocal8Bit()));
}

void InterfaceDSPIC::on_btnClearConsole_clicked()
{
    ui->teConsole->clear();
    ui->leMessageToSend->clear();
}

void InterfaceDSPIC::on_newMessage(QByteArray message)
{

}

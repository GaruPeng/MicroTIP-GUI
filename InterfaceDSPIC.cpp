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
    /* Hide Close Communication on startup */
    ui->btnCloseCommunicationWithMicrocontroller->setEnabled(false);

    /* Hide Emission/Reception groupbox on startup */
    ui->gpbCommuniationEmissionReception->setEnabled(false);
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
    ui->gpbCommuniationEmissionReception->setEnabled(true);
    ui->btnOpenCommunicationWithMicrocontroller->setEnabled(false);
    ui->btnCloseCommunicationWithMicrocontroller->setEnabled(true);
    connect(serial, SIGNAL(workDone(QByteArray)), this, SLOT(on_SerialCommunicationWorkDone(QByteArray)));
}

void InterfaceDSPIC::on_btnCloseCommunicationWithMicrocontroller_clicked()
{
    delete serial;
    ui->btnOpenCommunicationWithMicrocontroller->setEnabled(true);
    ui->btnCloseCommunicationWithMicrocontroller->setEnabled(false);
    ui->gpbCommuniationEmissionReception->setEnabled(false);
}

void InterfaceDSPIC::on_btnSendMessage_clicked()
{
    ui->teReceivedInformation->append(QString("Sent : " + ui->leMessageToSend->text()));
    serial->sendMessage(QByteArray::fromHex(ui->leMessageToSend->text().toLocal8Bit()));
}

void InterfaceDSPIC::on_btnClearTeReceivedInformation_clicked()
{
    ui->teReceivedInformation->clear();
}

void InterfaceDSPIC::on_SerialCommunicationWorkDone(QByteArray message)
{
    ui->teReceivedInformation->append(QString("Received : " + QString(message.toUpper())));
}

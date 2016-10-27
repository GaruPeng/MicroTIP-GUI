#include "InterfaceDSPIC.h"
#include "ui_InterfaceDSPIC.h"

InterfaceDSPIC::InterfaceDSPIC(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InterfaceDSPIC)
{
    ui->setupUi(this);
    updateSerialPorts();
    updateBaudrates();
}

InterfaceDSPIC::~InterfaceDSPIC()
{
    delete ui;
    delete serial;
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
}

void InterfaceDSPIC::on_btnCloseCommunicationWithMicrocontroller_clicked()
{
    delete serial;
}

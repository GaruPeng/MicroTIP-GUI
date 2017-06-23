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

    ui->gpbDac->setEnabled(false);
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
    dac = new Dac();

    ui->btnOpenCommunicationWithMicrocontroller->setEnabled(false);
    ui->btnCloseCommunicationWithMicrocontroller->setEnabled(true);
    ui->btnRefreshSerialNames->setEnabled(false);
    ui->cmbSerialNames->setEnabled(false);
    ui->cmbBaudRates->setEnabled(false);

    ui->btnSendMessage->setEnabled(true);
    ui->leMessageToSend->setEnabled(true);

    ui->gpbDac->setEnabled(true);

    connect(serial, SIGNAL(workDone(QByteArray)), this, SLOT(on_newMessage(QByteArray)));
}

void InterfaceDSPIC::on_btnCloseCommunicationWithMicrocontroller_clicked()
{
    delete serial;
    delete dac;

    ui->btnOpenCommunicationWithMicrocontroller->setEnabled(true);
    ui->btnCloseCommunicationWithMicrocontroller->setEnabled(false);
    ui->btnRefreshSerialNames->setEnabled(true);
    ui->cmbSerialNames->setEnabled(true);
    ui->cmbBaudRates->setEnabled(true);

    ui->btnSendMessage->setEnabled(false);
    ui->leMessageToSend->setEnabled(false);

    ui->leMessageToSend->clear();

    ui->gpbDac->setEnabled(false);
}

void InterfaceDSPIC::on_btnSendMessage_clicked()
{
    if(ui->leMessageToSend->text().isEmpty())
    {
        QMessageBox::critical(this, "", "Cannot send empty message");
    }
    else
    {
        ui->teConsole->append(QString("Sent : " + ui->leMessageToSend->text()));
        serial->sendMessage(QByteArray::fromHex(ui->leMessageToSend->text().toLocal8Bit()));
    }
}

void InterfaceDSPIC::on_btnClearConsole_clicked()
{
    ui->teConsole->clear();
    ui->leMessageToSend->clear();
}

void InterfaceDSPIC::on_newMessage(QByteArray message)
{
   CMD cmd((CMD)QString(message.left(2)).toInt(nullptr,16));
   message.remove(0,2);

   switch(cmd)
   {
   case CMD_DAC_GET_VALUE:
       dac->setValue(message.left(4).toInt(nullptr,16));
       ui->leDacValue->setText(QString::number(dac->getValue()));
       ui->teConsole->append("Current DAC value: " + ui->leDacValue->text());
       message.clear();
       break;
   default:
       break;
   }
}

void InterfaceDSPIC::on_btnDacGetValue_clicked()
{
    qDebug() << "Getting Dac Value";

    ui->teConsole->append("Retrieving DAC current value ...");
    QByteArray messageToSend;

    messageToSend.append((char)0x02);
    messageToSend.append((char)CMD_DAC_GET_VALUE);

    serial->sendMessage(messageToSend);
}

void InterfaceDSPIC::on_btnDacSetValue_clicked()
{
    if(ui->leDacValue->text().isEmpty())
    {
        QMessageBox::critical(this,"DAC","Cannot set an empty value");
    }
    else if(ui->leDacValue->text().toInt() > 65535)
    {
        QMessageBox::critical(this,"DAC","Cannot set a value > 65535");
    }
    else
    {
       dac->setValue(ui->leDacValue->text().toInt());

       qDebug() << "Setting Dac Value";
       ui->teConsole->append("Setting DAC value to " + QString::number(dac->getValue()));

       QByteArray messageToSend;

       messageToSend.append((char)0x04);
       messageToSend.append((char)CMD_DAC_SET_VALUE);
       messageToSend.append((char)(dac->getValue() >> 8));
       messageToSend.append((char)dac->getValue());

       serial->sendMessage(messageToSend);
    }

}

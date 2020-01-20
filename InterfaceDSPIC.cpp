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

    haptic = new Haptic();
    timerPos = new QTimer;
    timerPos->setInterval(1);
    timerPos->start();
    connect(timerPos,SIGNAL(timeout()),this,SLOT(showPosMonitor()));

    /* Create a scope window */
    scope = new Scope;
    scope->setGeometry(50,50,700,300);
    //scope->show();
}


InterfaceDSPIC::~InterfaceDSPIC()
{
    delete ui;
    delete serial;
    delete haptic;
}


void InterfaceDSPIC::init()
{
    /* Disable Close Communication on startup */
    ui->btnCloseCommunicationWithMicrocontroller->setEnabled(false);

    /* Disable Emission/Reception groupbox on startup */
    ui->leMessageToSend->setEnabled(false);
    ui->btnSendMessage->setEnabled(false);
    ui->gpbDac->setEnabled(false);


    /* Disable Position Monitor on startup */
    ui->spbPosQW->setEnabled(false);
    ui->spbPosQX->setEnabled(false);
    ui->spbPosQY->setEnabled(false);
    ui->spbPosQZ->setEnabled(false);
    ui->spbPosX->setEnabled(false);
    ui->spbPosY->setEnabled(false);
    ui->spbPosZ->setEnabled(false);

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
    ui->teConsole->append(serial->retext);

    dac = new Dac();

    mux = new Multiplexer();

    pwm = new PWM();

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
    delete mux;

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
    message.remove(0,2);
    CMD cmd((CMD)QString(message.left(2)).toInt(nullptr,16));

    switch(cmd) /* The 2nd byte of message */
    {
    case CMD_DAC_GET_VALUE:
        dac->setValue(message.right(4).toInt(nullptr,16));
        ui->leDacValue->setText(QString::number(dac->getValue()));
        ui->teConsole->append("Current DAC value: " + ui->leDacValue->text());
        message.clear();
        break;
    case CMD_DAC_SET_VALUE:
        ui->teConsole->append("DAC value is set ");
        message.clear();
        break;
    case CMD_PWM_GET_FREQ:
        pwm->setFreq(message.right(4).toInt(nullptr,16));
        ui->lePwmFreq->setText(QString::number(pwm->getFreq()));
        ui->teConsole->append("Current PWM frequency: " + ui->lePwmFreq->text());
        message.clear();
        break;
    case CMD_PWM_SET_FREQ:
        ui->teConsole->append("Frequency is set ");
        message.clear();
        break;
    case CMD_PWM_GET_DUTY:
        pwm->setDuty(message.right(2).toInt(nullptr,16));
        ui->lePwmDuty->setText(QString::number(pwm->getDuty()));
        ui->teConsole->append("Current PWM duty cycle: " + QString::number(pwm->getDuty()) + "0%");
        message.clear();
        break;
    case CMD_PWM_SET_DUTY:
        ui->teConsole->append("Duty cycle is set ");
        message.clear();
        break;
    default:
        break;
    }
}


void InterfaceDSPIC::on_btnDacGetValue_clicked()
{
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


void InterfaceDSPIC::on_btnPwmGetFreq_clicked()
{
    QByteArray messageToSend;

    messageToSend.append((char)0x02);
    messageToSend.append((char)CMD_PWM_GET_FREQ);

    serial->sendMessage(messageToSend);
}


void InterfaceDSPIC::on_btnPwmSetFreq_clicked()
{
    if(ui->lePwmFreq->text().isEmpty())
    {
        QMessageBox::critical(this,"PWM","Cannot set an empty value");
    }

    else
    {
        pwm->setFreq(ui->lePwmFreq->text().toInt());

        qDebug() << "Setting PWM Frequency";
        ui->teConsole->append("Setting PWM freqency to " + QString::number(pwm->getFreq()));

        QByteArray messageToSend;

        messageToSend.append((char)0x04);
        messageToSend.append((char)CMD_PWM_SET_FREQ);
        messageToSend.append((char)(pwm->getFreq() >> 8));
        messageToSend.append((char)pwm->getFreq());

        serial->sendMessage(messageToSend);
    }
}


void InterfaceDSPIC::on_btnPwmGetDuty_clicked()
{
    QByteArray messageToSend;

    messageToSend.append((char)0x02);
    messageToSend.append((char)CMD_PWM_GET_DUTY);

    serial->sendMessage(messageToSend);
}


void InterfaceDSPIC::on_btnPwmSetDuty_clicked()
{
    if(ui->lePwmDuty->text().isEmpty())
    {
        QMessageBox::critical(this,"PWM","Cannot set an empty value");
    }
    else if(ui->lePwmDuty->text().toInt() > 10000)
    {
        QMessageBox::critical(this,"PWM","Cannot set a duty cycle > 100%");
    }
    else
    {
        pwm->setDuty(ui->lePwmDuty->text().toInt());

        qDebug() << "Setting PWM Duty Cycle";
        ui->teConsole->append("Setting PWM Duty Cycle to " + QString::number(pwm->getDuty()) + "0%");

        QByteArray messageToSend;

        messageToSend.append((char)0x03);
        messageToSend.append((char)CMD_PWM_SET_DUTY);
        //messageToSend.append((char)(pwm->getDuty() >> 8));
        messageToSend.append((char)pwm->getDuty());

        serial->sendMessage(messageToSend);
    }
}


void InterfaceDSPIC::on_btgMux_buttonClicked(int id)
{
    QByteArray messageToSend;
    messageToSend.append(0x03);
    messageToSend.append(CMD_MUX_SET_VALUE);

    mux->setInput((char)(-id-2));
    messageToSend.append(mux->getInput());

    serial->sendMessage(messageToSend);
    ui->teConsole->append("Setting MUX input to : " + QString::number(mux->getInput()));
}


void InterfaceDSPIC::on_btnOpenHapticComm_clicked()
{
    QString cr;
    cr = QString(haptic->OpenCommunication());
    ui->teConsole->append("Haptic status : " + cr);
}


void InterfaceDSPIC::on_btnCloseHapticComm_clicked()
{
    QString cr;
    cr = QString(haptic->CloseCommunication());
    ui->teConsole->append("Haptic status : " + cr);
}


void InterfaceDSPIC::on_btnStartHapticSim_clicked()
{
    QString cr;
    //ui->btnHapticSetForce->setEnabled(false);
    cr = QString(haptic->StartSpringSim());
    ui->teConsole->append("Haptic status : " + cr);
}

void InterfaceDSPIC::on_btnStopHapticSim_clicked()
{
    QString cr;
    //ui->btnHapticSetForce->setEnabled(true);
    cr = QString(haptic->StopSpringSim());
    ui->teConsole->append("Haptic status : " + cr);
}


void InterfaceDSPIC::on_btnExit_clicked()
{
    exit(1);
}


void InterfaceDSPIC::on_btnHapticSetForce_clicked()
{
    QString cr;
    float newforce[6];
    newforce[0] = (float)0.001*ui->spbAxe1->value();
    newforce[1] = (float)0.001*ui->spbAxe2->value();
    newforce[2] = (float)0.001*ui->spbAxe3->value();
    newforce[3] = (float)0.001*ui->spbAxe4->value();
    newforce[4] = (float)0.001*ui->spbAxe5->value();
    newforce[5] = (float)0.001*ui->spbAxe6->value();

    cr = QString(haptic->SetForce(newforce));
    ui->teConsole->append("Haptic status : " + cr);

}


void InterfaceDSPIC::on_btnHapticDefaultForce_clicked()
{
    /* Set the default forces and torques */
    ui->spbAxe1->setValue(100); /* fx */
    ui->spbAxe2->setValue(100); /* fy */
    ui->spbAxe3->setValue(100); /* fz */
    ui->spbAxe4->setValue(0);   /* cx */
    ui->spbAxe5->setValue(0);   /* cy */
    ui->spbAxe6->setValue(0);   /* cz */
}


void InterfaceDSPIC::on_btnHapticLoopForce_clicked()
{
    QString cr;

    float newforce[6];
    for(int i = 0 ; i < 6 ; i++)
    {
        newforce[i] = 0;
    }

    /* Be careful to change the value */
    /* It is dangerous if the forces increases too fast */
    for(int ii = 0 ; ii < 200 ; ii+=10)
    {
        newforce[0] = (float)0.001*ii;
        //cr = QString(haptic->SetForce(newforce));
        cr = QString("New force: %1, %2, %3, %4, %5, %6").arg(newforce[0]).arg(newforce[1])
                .arg(newforce[2]).arg(newforce[3]).arg(newforce[4]).arg(newforce[5]);
        ui->teConsole->append("Haptic status : " + cr);

        /* Wait 1 sec for each loop */
        QTime t;
        t.start();
        while(t.elapsed()<1000)
            QCoreApplication::processEvents();
    }
}


void InterfaceDSPIC::showPosMonitor()
{
    ui->spbPosX->setValue(haptic->position[0]);
    ui->spbPosY->setValue(haptic->position[1]);
    ui->spbPosZ->setValue(haptic->position[2]);
    ui->spbPosQX->setValue(haptic->position[3]);
    ui->spbPosQY->setValue(haptic->position[4]);
    ui->spbPosQZ->setValue(haptic->position[5]);
    ui->spbPosQW->setValue(haptic->position[6]);
}


void InterfaceDSPIC::on_ckbPosSet_clicked(bool checked)
{
    if(checked==true)
    {
        ui->spbPosQW->setEnabled(true);
        ui->spbPosQX->setEnabled(true);
        ui->spbPosQY->setEnabled(true);
        ui->spbPosQZ->setEnabled(true);
        ui->spbPosX->setEnabled(true);
        ui->spbPosY->setEnabled(true);
        ui->spbPosZ->setEnabled(true);
    }

    else
    {
        ui->spbPosQW->setEnabled(false);
        ui->spbPosQX->setEnabled(false);
        ui->spbPosQY->setEnabled(false);
        ui->spbPosQZ->setEnabled(false);
        ui->spbPosX->setEnabled(false);
        ui->spbPosY->setEnabled(false);
        ui->spbPosZ->setEnabled(false);
    }
}



void InterfaceDSPIC::on_btnPwmReset_clicked()
{
    qDebug() << "Reset PWM";
    ui->teConsole->append("Reset PWM value to 0V");

    QByteArray messageToSend;

    messageToSend.append((char)0x02);
    messageToSend.append((char)0x80);


    serial->sendMessage(messageToSend);
}

void InterfaceDSPIC::on_btnPwmRun_clicked()
{
    qDebug() << "PWM default";
    //ui->teConsole->append("Reset PWM value to 0V");

    QByteArray messageToSend;
    QByteArray messageToSend2;

    messageToSend.append((char)0x02);
    messageToSend.append((char)0x82);
    serial->sendMessage(messageToSend);

    messageToSend2.append((char)0x04);
    messageToSend2.append((char)CMD_DAC_SET_VALUE);
    messageToSend2.append((char)(65535 >> 8));
    messageToSend2.append((char)65535);
    serial->sendMessage(messageToSend2);
}

void InterfaceDSPIC::on_btnPwmWarm_clicked()
{
    qDebug() << "Warm PWM";
    ui->teConsole->append("PWM 0V activation");

    QByteArray messageToSend;

    messageToSend.append((char)0x02);
    messageToSend.append((char)0x81);


    serial->sendMessage(messageToSend);
}

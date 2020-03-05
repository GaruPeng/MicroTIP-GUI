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


    timerPos = new QTimer;
    timerPos->setInterval(1);
    //timerPos->start();
    connect(timerPos,SIGNAL(timeout()),this,SLOT(ReadBCIvalue()));

    timerS = new QTimer;
    timerS->setInterval(1);

    timers = new QTimer;
    timers->setInterval(1);

    timerG = new QTimer;
    timerG->setInterval(100);
    //updateTimeG




    /* Create a scope window */
    scope = new Scope;
    scope->setGeometry(50,50,700,300);
    scope->show();
    //connect(bci,SIGNAL(EEGvalue(double)),scope,SLOT(Record(double)));
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
    ui->ckbDriveBCI->setEnabled(false);
    ui->ckbReadBCI->setEnabled(false);
    ui->lcdDacValue->setEnabled(false);
    ui->hsDacValue->setEnabled(false);
    ui->btnGrip->setEnabled(false);

    ui->lcdGtime->display("00:00:00:000");

    ui->lcdGtime->setPalette(Qt::black);
    ui->lcdDacValue->setPalette(Qt::black);
    ui->lcdAttention->setPalette(Qt::black);



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

    bci = new BCI();

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






/*--------------------DAC------------------------------*/

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

void InterfaceDSPIC::on_btnExit_clicked()
{
    exit(1);
}






/*--------------------Mux------------------------------*/

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


/*--------------------Haptic---------------------------*/


/*--------------------PWM------------------------------*/

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
    messageToSend2.append((char)(45874 >> 8));
    messageToSend2.append((char)45874);
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






/*--------------------BCI--------------------------*/

void InterfaceDSPIC::on_btnConnectBCI_clicked()
{
    ui->teConsole->append(bci->OpenBCIconnection());
    ui->ckbReadBCI->setEnabled(true);


}

void InterfaceDSPIC::on_btnDIsconnectBCI_clicked()
{
    ui->teConsole->append(bci->CloseBCIconnection());
    ui->ckbReadBCI->setEnabled(false);
}



void InterfaceDSPIC::on_ckbReadBCI_clicked(bool checked)
{
    if(checked==1)
    {
        ui->btnConnectBCI->setEnabled(false);
        ui->btnDIsconnectBCI->setEnabled(false);
        ui->ckbDriveBCI->setEnabled(true);
        timerPos->start();
    }

    if(checked==0)
    {
        ui->btnConnectBCI->setEnabled(true);
        ui->btnDIsconnectBCI->setEnabled(true);
        ui->ckbDriveBCI->setEnabled(false);
        timerPos->stop();
    }

}

void InterfaceDSPIC::ReadBCIvalue()
{
    bci->ReadBCItest();

    ui->lcdAttention->display(bci->attenlvl);

}

void InterfaceDSPIC::DriveBCIvalue()
{
    double attOuput;
    attOuput = (double) bci->attenlvl;


    if (attOuput > 80){
        ui->leDacValue->setText(QString::number(45874));
    }
    if (attOuput < 30){
        ui->leDacValue->setText(QString::number(0));
    }
    if(attOuput>30 && attOuput<80){
        attOuput = (attOuput-30) *45874/50;
        ui->leDacValue->setText(QString::number((uint16_t) attOuput));
    }


}


void InterfaceDSPIC::on_ckbDriveBCI_clicked(bool checked)
{
    if(checked==1)
    {
        ui->ckbReadBCI->setEnabled(false);
        connect(timerPos,SIGNAL(timeout()),this,SLOT(DriveBCIvalue()));
        connect(timerPos,SIGNAL(timeout()),this,SLOT(on_btnDacSetValue_clicked()));

    }

    if(checked==0)
    {
        ui->ckbReadBCI->setEnabled(true);
        disconnect(timerPos,SIGNAL(timeout()),this,SLOT(DriveBCIvalue()));
        disconnect(timerPos,SIGNAL(timeout()),this,SLOT(on_btnDacSetValue_clicked()));

    }
}






/*--------------------SquareRun--------------------------*/

void InterfaceDSPIC::on_btnSquareRun_clicked()
{
    ui->lcdDacValue->setEnabled(true);
    ui->hsDacValue->setEnabled(true);
    connect(timerS,SIGNAL(timeout()),this,SLOT(SquareUP()));
    connect(timers,SIGNAL(timeout()),this,SLOT(SquareDown()));

    timerS->start(5000);



}

void InterfaceDSPIC::on_btnSquareStop_clicked()
{
    timerS->stop();
    timers->stop();
    disconnect(timerS,SIGNAL(timeout()),this,SLOT(SquareUP()));
    disconnect(timers,SIGNAL(timeout()),this,SLOT(SquareDown()));
    ui->lcdDacValue->setEnabled(false);
    ui->hsDacValue->setEnabled(false);

    ui->leDacValue->setText(QString::number(32767));
    ui->btnDacSetValue->click();
}


void InterfaceDSPIC::SquareUP()
{
    double valueUP;
    valueUP = (double)ui->hsDacValue->value()/25*32767+32767;
    ui->leDacValue->setText(QString::number((int)valueUP));
    ui->btnDacSetValue->click();
    timers->start(5000);

}

void InterfaceDSPIC::SquareDown()
{
    double valueDOWN;
    valueDOWN = 32767-(double)ui->hsDacValue->value()/25*32767;
    ui->leDacValue->setText(QString::number((int)valueDOWN));
    ui->btnDacSetValue->click();
    timerS->start(5000);
}

void InterfaceDSPIC::on_hsDacValue_valueChanged(int value)
{
    double voltage;
    voltage = ((double) value)/10;
    ui->lcdDacValue->display(voltage);
    //ui->leDacValue->setText(QString::number((int)voltage/25*65535));
}







/*--------------------Grip-----------------------------*/


void InterfaceDSPIC::on_ckbGrip_clicked(bool checked)
{
    if(checked==1){
        //ui->lcdDacValue->setEnabled(false);
        //ui->hsDacValue->setEnabled(false);

        ui->leDacValue->setText(QString::number(13106)); // -1.5V open the gripper
        ui->btnDacSetValue->click();

        ui->btnGrip->setEnabled(true);

        ui->tbGrip->append("Open the gripper at the beginning");


    }

    if(checked==0){
        //ui->lcdDacValue->setEnabled(true);
        //ui->hsDacValue->setEnabled(true);


        ui->btnGrip->setEnabled(false);

        timerG->stop();

        disconnect(timerG,SIGNAL(timeout()),this,SLOT(updateTimeG()));

        ui->leDacValue->setText(QString::number(32767)); // go back to 0V
        ui->btnDacSetValue->click();

        ui->tbGrip->clear();

    }
}


void InterfaceDSPIC::on_btnGrip_clicked()
{
    EXPtime.start();
    timerG->start();

    connect(timerG,SIGNAL(timeout()),this,SLOT(updateTimeG()));


}

void InterfaceDSPIC::updateTimeG()
{
    int stateT = 0;
    QTime showTime(0,0,0,0);
    QString timeStr;
    showTime = showTime.addMSecs(EXPtime.elapsed());
    timeStr = showTime.toString("hh:mm:ss:zzz");
    ui->lcdGtime->display(timeStr);

    // wait 5 sec
    if(EXPtime.hasExpired(5000)==1)
    {
        stateT = 1;

        if(EXPtime.hasExpired(45000)==1){ // close the gripper for 40 sec
            stateT = 2;

            if(EXPtime.hasExpired(60000)==1){ // reopen the gripper for 15 sec
                stateT = 3;

                if(EXPtime.hasExpired(62000)==1){  // go to the neutral state
                    stateT = 4;

                    if(EXPtime.hasExpired(70000)){
                        stateT = 5;
                    }
                }
            }
        }
    }

    switch (stateT) {
    case 1:
        if(dac->getValue()!=58981){
            ui->leDacValue->setText(QString::number(58981)); // +2.0V close the gripper
            ui->btnDacSetValue->click();
            ui->tbGrip->append("Close the gripper at 5 sec");
        }
        break;
    case 2:
        if(dac->getValue()!=13106){
            ui->leDacValue->setText(QString::number(13106)); // -1.5V open the gripper
            ui->btnDacSetValue->click();
            ui->tbGrip->append("Open the gripper at 45 sec");
        }
        break;
    case 3:
        if(dac->getValue()!=45874){
            ui->leDacValue->setText(QString::number(45874)); // +1.0V close the gripper
            ui->btnDacSetValue->click();
            ui->tbGrip->append("Go back to neutral state");
        }
        break;
    case 4:
        if(dac->getValue()!=32767){
            ui->leDacValue->setText(QString::number(32767)); // +0.0V open the gripper
            ui->btnDacSetValue->click();
        }
        break;
    case 5:
        timerG->stop();
        break;
    default:
        break;
    }




}

void InterfaceDSPIC::GripExp()
{

}

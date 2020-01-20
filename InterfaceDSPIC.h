#ifndef INTERFACEDSPIC_H
#define INTERFACEDSPIC_H

#include <QDialog>
#include <QMessageBox>
#include <QTime>
#include <QTimer>

#include "SerialCommunication.h"
#include "Dac.h"
#include "Multiplexer.h"
#include "haptic.h"
#include "scope.h"
#include "Pwm.h"


namespace Ui {
class InterfaceDSPIC;
}

class InterfaceDSPIC : public QDialog
{
    Q_OBJECT
public:
    explicit InterfaceDSPIC(QWidget *parent = 0);
    ~InterfaceDSPIC();
    void init();
    void updateSerialPorts();
    void updateBaudrates();

public slots:
    void on_btnRefreshSerialNames_clicked();
    void on_btnOpenCommunicationWithMicrocontroller_clicked();
    void on_btnCloseCommunicationWithMicrocontroller_clicked();
    void on_btnSendMessage_clicked();
    void on_btnClearConsole_clicked();
    void on_newMessage(QByteArray);
    void on_btnDacGetValue_clicked();
    void on_btnDacSetValue_clicked();
    void on_btnPwmGetFreq_clicked();
    void on_btnPwmSetFreq_clicked();
    void on_btnPwmGetDuty_clicked();
    void on_btnPwmSetDuty_clicked();
    void on_btgMux_buttonClicked(int id);

    void on_btnOpenHapticComm_clicked();
    void on_btnCloseHapticComm_clicked();
    void on_btnStartHapticSim_clicked();
    void on_btnStopHapticSim_clicked();

private slots:
    void on_btnExit_clicked();
    void on_btnHapticSetForce_clicked();
    void on_btnHapticDefaultForce_clicked();
    void on_btnHapticLoopForce_clicked();
    void showPosMonitor();
    void on_ckbPosSet_clicked(bool checked);


    void on_btnPwmReset_clicked();

    void on_btnPwmRun_clicked();

    void on_btnPwmWarm_clicked();

signals:


private:
    Ui::InterfaceDSPIC *ui;
    SerialCommunication *serial;
    Dac *dac;
    Multiplexer *mux;
    Haptic *haptic;
    QTimer *timerPos;
    Scope *scope;
    PWM *pwm;


};

typedef enum
{
    CMD_DAC_SET_VALUE = 0x10,
    CMD_DAC_GET_VALUE = 0x11,
    CMD_MUX_SET_VALUE = 0x20,
    CMD_PWM_SET_FREQ = 0x30,
    CMD_PWM_GET_FREQ = 0x31,
    CMD_PWM_SET_DUTY = 0x40,
    CMD_PWM_GET_DUTY = 0x41,
    CMD_PWM_STOP     = 0x80,
    CMD_PWM_WARMUP   = 0x81,
    CMD_PWM_START    = 0x82
}CMD;

#endif // INTERFACEDSPIC_H

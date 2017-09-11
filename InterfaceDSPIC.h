#ifndef INTERFACEDSPIC_H
#define INTERFACEDSPIC_H

#include <QDialog>
#include <QMessageBox>

#include "SerialCommunication.h"
#include "Dac.h"
#include "Multiplexer.h"

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
    void on_btgMux_buttonClicked(int id);
private:
    Ui::InterfaceDSPIC *ui;
    SerialCommunication *serial;
    Dac *dac;
    Multiplexer *mux;
};

typedef enum
{
    CMD_DAC_SET_VALUE = 0x10,
    CMD_DAC_GET_VALUE = 0x11,
    CMD_MUX_SET_VALUE = 0x20,
}CMD;

#endif // INTERFACEDSPIC_H

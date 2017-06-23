#ifndef INTERFACEDSPIC_H
#define INTERFACEDSPIC_H

#include <QDialog>
#include <QMessageBox>

#include "SerialCommunication.h"
#include "Dac.h"

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
private:
    Ui::InterfaceDSPIC *ui;
    SerialCommunication *serial;
    Dac *dac;
};

#endif // INTERFACEDSPIC_H

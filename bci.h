#ifndef BCI_H
#define BCI_H

#include <QObject>
#include <QLibrary>
#include <QDebug>
#include <QMessageBox>
#include <QThread>
extern "C"
{
#include "thinkgear.h"
}

class BCI : public QObject
{
    Q_OBJECT
public:
    BCI();
    QString OpenBCIconnection(void);
    QString CloseBCIconnection(void);
    void ReadBCI(void);
    void ReadBCItest(void);
    int attenlvl;
    int rawEEG;


private:
    bool isConnect;
    bool isRunning;
    int connectId;
    int errcode;
    int DataDraw;
    int DataValue;
    int DataRec;

signals:
    void EEGvalue(int);



};

#endif // BCI_H

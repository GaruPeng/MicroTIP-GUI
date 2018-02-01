#include "haptic.h"

Haptic::Haptic()
{
    VC = NULL;
    isConnected = false;
    isRunning = false;

    /* Initialize force data */
    for(int i = 0 ; i < 6 ; i++)
        forces[i] = 0.f ;
    /* Initialize position data */
    for(int i = 0 ; i < 7 ; i++)
        position[i] = 0.f ;
}

Haptic::~Haptic()
{

}

/* Declare static variables */
float Haptic::forces[6];
float Haptic::position[7];


void Haptic::update_haptic(VirtContext VC, void *)
{
    int cr = 0;
    cr = virtSetForce(VC, forces);
    virtGetPosition(VC,position);
}


QString Haptic::OpenCommunication(void)
{
    QString cr;
    /* Address should be checked in PC */
    VC = NULL;
    VC = virtOpen("127.0.0.1#256");

    if (VC == NULL)
        cr = (QString) virtGetErrorMessage(virtGetErrorCode(NULL));
    else {
        isConnected = true;
        cr = QString("Success to connect");
    }

    qDebug() << "isConnected= " + QString::number(isConnected)
                + ", isRunisRunning= " + QString::number(isRunning);
    return cr;
}


QString Haptic::CloseCommunication(void)
{
    QString cr;
    int chk = virtClose(VC);

    if (chk != 0)
        cr = (QString)virtGetErrorMessage(virtGetErrorCode(VC));
    else {
        isConnected = false;
        cr = QString("Communication has been closed");
    }

    qDebug() << "virtClose= " + QString::number(chk) + ", isConnected= " + QString::number(isConnected)
                + ", isRunisRunning= " + QString::number(isRunning);
    return cr;
}


QString Haptic::StartSpringSim(void)
{
    QString cr;
    int chk = 0;

    if(!isConnected)
    {
        qDebug() << "chk= " + QString::number(chk) + ", isConnected= " + QString::number(isConnected)
                    + ", isRunisRunning= " + QString::number(isRunning);
        return cr = (QString)"Please connect first";
    }
    if(isRunning)
    {
        qDebug() << "chk= " + QString::number(chk) + ", isConnected= " + QString::number(isConnected)
                    + ", isRunisRunning= " + QString::number(isRunning);
        return cr = (QString)"A simulation is running";
    }
    else {
        simu_step_calc = .001f;
        virtSetIndexingMode(VC, INDEXING_ALL);
        virtSetForceFactor(VC, 1.0f);
        virtSetSpeedFactor(VC, 1.0f);
        virtSetTimeStep(VC, simu_step_calc);

        chk = virtSetCommandType(VC, COMMAND_TYPE_IMPEDANCE);
        if (chk != 0)
        {
            qDebug() << "virtSetCommandType= " + QString::number(chk) + ", isConnected= " + QString::number(isConnected)
                        + ", isRunisRunning= " + QString::number(isRunning);
            return cr = (QString)virtGetErrorMessage(virtGetErrorCode(VC));
        }

        chk = virtSetPowerOn(VC, true);
        if (chk != 0)
        {
            qDebug() << "virtSetPowerOn= " + QString::number(chk) + ", isConnected= " + QString::number(isConnected)
                        + ", isRunisRunning= " + QString::number(isRunning);
            return cr = (QString)virtGetErrorMessage(virtGetErrorCode(VC));
        }

        chk = virtSetPeriodicFunction(VC, update_haptic, &simu_step_calc, this);
        if (chk != 0)
        {
            qDebug() << "virtSetPeriodicFunction= " + QString::number(chk) + ", isConnected= " + QString::number(isConnected)
                        + ", isRunisRunning= " + QString::number(isRunning);
            return cr = (QString)virtGetErrorMessage(virtGetErrorCode(VC));
        }

        chk = virtStartLoop(VC);
        if (chk != 0)
        {
            qDebug() << "virtStartLoop= " + QString::number(chk) + ", isConnected= " + QString::number(isConnected)
                        + ", isRunisRunning= " + QString::number(isRunning);
            return cr = (QString)virtGetErrorMessage(virtGetErrorCode(VC));
        }

        isRunning = true;

        qDebug() << "chk= " + QString::number(chk) + ", isConnected= " + QString::number(isConnected)
                    + ", isRunisRunning= " + QString::number(isRunning);
        return cr = (QString)"Simulation has been started";
    }
}


QString Haptic::StopSpringSim(void)
{
    QString cr;
    int chk = 0;

    if (!isRunning)
    {
        qDebug() << "chk= " + QString::number(chk) + ", isConnected= " + QString::number(isConnected)
                    + ", isRunisRunning= " + QString::number(isRunning);
        return cr = (QString)"Nothing to stop";
    }
    else
    {
        chk = virtStopLoop(VC);
        if(chk != 0)
        {
            qDebug() << "virtStopLoop= " + QString::number(chk) + ", isConnected= " + QString::number(isConnected)
                        + ", isRunisRunning= " + QString::number(isRunning);
            return cr = (QString)virtGetErrorMessage(virtGetErrorCode(VC));
        }

        chk = virtSetPowerOn(VC, false);
        if(chk != 0)
        {
            qDebug() << "virtSetPowerOn= " + QString::number(chk) + ", isConnected= " + QString::number(isConnected)
                        + ", isRunisRunning= " + QString::number(isRunning);
            return cr = (QString)virtGetErrorMessage(virtGetErrorCode(VC));
        }

        isRunning = false;
        qDebug() << "chk= " + QString::number(chk) + ", isConnected= " + QString::number(isConnected)
                    + ", isRunisRunning= " + QString::number(isRunning);
        return cr = (QString)"Simulation is over";
    }
}


QString Haptic::SetForce(float newforce[6])
{
    QString cr;

    if(!isConnected)
    {
        qDebug() << "isConnected= " + QString::number(isConnected)
                    + ", isRunisRunning= " + QString::number(isRunning);
        return cr = (QString)"Please connect first";
    }
/*
    if(isRunning)
        return cr = (QString)"Please stop the simulation";
*/
    else
    {
        int chk = virtStopLoop(VC);
        if(chk != 0)
        {
            qDebug() << "virtStopLoop= " + QString::number(chk) + ", isConnected= " + QString::number(isConnected)
                        + ", isRunisRunning= " + QString::number(isRunning);
            return cr = (QString)virtGetErrorMessage(virtGetErrorCode(VC));
        }

        /* Change to new force setting */
        for(int i = 0 ; i < 6 ; i++)
            forces[i] = newforce[i];

        chk = virtSetPeriodicFunction(VC, update_haptic, &simu_step_calc, this);
        if (chk != 0)
        {
            qDebug() << "chk= " + QString::number(chk) + ", isConnected= " + QString::number(isConnected)
                        + ", isRunisRunning= " + QString::number(isRunning);
            return cr = (QString)virtGetErrorMessage(virtGetErrorCode(VC));
        }
        else
        {
            chk = virtStartLoop(VC);
            if (chk != 0)
            {
                qDebug() << "virtStartLoop= " + QString::number(chk) + ", isConnected= " + QString::number(isConnected)
                            + ", isRunisRunning= " + QString::number(isRunning);
                return cr = (QString)virtGetErrorMessage(virtGetErrorCode(VC));
            }

            qDebug() << "chk= " + QString::number(chk) + ", isConnected= " + QString::number(isConnected)
                        + ", isRunisRunning= " + QString::number(isRunning);
            return cr = QString("New force: %1, %2, %3, %4, %5, %6").arg(forces[0]).arg(forces[1])
                    .arg(forces[2]).arg(forces[3]).arg(forces[4]).arg(forces[5]);
        }
    }
}



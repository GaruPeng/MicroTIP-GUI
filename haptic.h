#ifndef HAPTIC_H
#define HAPTIC_H

#include <QObject>
#include "VirtuoseAPI.h"
#include <QDebug>

class Haptic : public QObject
{
    Q_OBJECT
public:
    Haptic();
    ~Haptic();
    QString OpenCommunication(void);
    QString CloseCommunication(void);
    QString StartSpringSim(void);
    QString StopSpringSim(void);
    QString SetForce(float newforce[6]);
    static float forces[6];
    static float position[7];
    static void update_haptic(VirtContext VC, void*);

private:
    VirtContext VC;
    bool isConnected;
    bool isRunning;
    float simu_step_calc;

signals:


public slots:

};

#endif // HAPTIC_H

#ifndef THERMALCONTROL_H
#define THERMALCONTROL_H

#include <QObject>

#include "servocontrol.h"

class ThermalControl : public QObject
{
    Q_OBJECT
public:
    explicit ThermalControl();
    
signals:
    
public slots:

private:
    ServoControl servoControl;
};

#endif // THERMALCONTROL_H

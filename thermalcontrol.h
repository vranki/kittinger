#ifndef THERMALCONTROL_H
#define THERMALCONTROL_H

#include <QObject>
#include <QTimer>
#include "servocontrol.h"

#define TEMP_LOW 10 // Close cooling fully at this temp
#define TEMP_HIGH 60 // Open cooling fully at this temp
#define COOLING_SERVO 2

class ThermalControl : public QObject {
    Q_OBJECT
public:
    explicit ThermalControl();
    void init();
signals:
    void ready(bool is);
public slots:
    void temperatureChanged(int newTemp);
    void setFailsafe();
private:
    ServoControl servoControl;
    int lastTemp;
    QTimer failureTimer;
};

#endif // THERMALCONTROL_H

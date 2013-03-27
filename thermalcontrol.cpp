#include "thermalcontrol.h"

ThermalControl::ThermalControl() :  QObject(), lastTemp(-666) {
}

void ThermalControl::init() {
    if(servoControl.init(50,220)) {
        connect(&failureTimer, SIGNAL(timeout()), this, SLOT(setFailsafe()));
        failureTimer.setSingleShot(false);
        failureTimer.setInterval(10*60*1000); // 10 minutes
        setFailsafe();
        emit ready(true);
    } else {
        emit ready(false);
    }
}

void ThermalControl::temperatureChanged(int newTemp) {
    failureTimer.start();
    if(newTemp == lastTemp) return;
    if(newTemp < TEMP_LOW) {
        servoControl.setServo(COOLING_SERVO, 0);
    } else if(newTemp > TEMP_HIGH) {
        servoControl.setServo(COOLING_SERVO, 1);
    } else {
        float newPos = (float) (newTemp - TEMP_LOW) / (float) (TEMP_HIGH - TEMP_LOW);
        servoControl.setServo(COOLING_SERVO, newPos);
    }
    lastTemp = newTemp;
}

void ThermalControl::setFailsafe() {
    failureTimer.start();
    servoControl.setServo(COOLING_SERVO, 0.2f);
}

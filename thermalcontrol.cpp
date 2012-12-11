#include "thermalcontrol.h"

ThermalControl::ThermalControl() :  QObject() {
    servoControl.init(50,220);
    servoControl.setServo(2, 0.5f);
}

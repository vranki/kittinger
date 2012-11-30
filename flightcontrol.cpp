#include "flightcontrol.h"
#include <cmath>

FlightControl::FlightControl(QObject *parent) : QObject(parent) {
    setObjectName("FlightControl");
    lat = lon = alt = NAN;
    gpsStatus = STATUS_NO_FIX;
    takeoff.addTransition(this, SIGNAL(takeOffDetected()), &ascend);
    ascend.addTransition(this, SIGNAL(descendDetected()), &descend);
    ascend.addTransition(this, SIGNAL(survivalDetected()), &survival);
    descend.addTransition(this, SIGNAL(landedDetected()), &landed);
    descend.addTransition(this, SIGNAL(survivalDetected()), &survival);
    landed.addTransition(this, SIGNAL(survivalDetected()), &survival);
    machine.addState(&takeoff);
    machine.addState(&ascend);
    machine.addState(&descend);
    machine.addState(&landed);
    machine.addState(&survival);
    machine.setInitialState(&takeoff);
    machine.start();
}

void FlightControl::sendStatus() {
    QString status;
    status = QString("%1,%2,%3,%4").arg(lat).arg(lon).arg(alt).arg(GPSManager::statusString(gpsStatus));
    emit sendSms(status);
    emit log(status);
}

void FlightControl::statusChanged(GpsStatus status) {
    gpsStatus = status;
    emit log("GPS status now " + GPSManager::statusString(gpsStatus));
}

void FlightControl::gpsFix(double latitude, double longitude, double altitude) {
    lat = latitude;
    lon = longitude;
    alt = altitude;
}

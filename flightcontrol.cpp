#include "flightcontrol.h"
#include <cmath>
#include <QDebug>

FlightControl::FlightControl(QObject *parent) : QObject(parent) {
    setObjectName("FlightControl");
    lat = lon = alt = NAN;
    gpsStatus = STATUS_NO_FIX;
    messageSendInterval = 0;

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

    connect(&takeoff, SIGNAL(entered()), this, SLOT(startTakeoff()));
    connect(&gsmPollTimer, SIGNAL(timeout()), this, SIGNAL(pollGsm()));

    machine.start();
}

void FlightControl::sendStatus() {
    QString status;
    status = QString("%1,%2,%3,%4").arg(lat).arg(lon).arg(alt).arg(GPSManager::statusString(gpsStatus));
    emit sendSms(status);
    emit log(status);
    lastMessageSentTime = QTime::currentTime();
}

void FlightControl::statusChanged(GpsStatus status) {
    gpsStatus = status;
    emit log("GPS status now " + GPSManager::statusString(gpsStatus));
}

void FlightControl::gpsFix(double latitude, double longitude, double altitude) {
    lat = latitude;
    lon = longitude;
    alt = altitude;
    if(messageSendInterval > 0 && QTime::currentTime() > lastMessageSentTime.addSecs(messageSendInterval))
        sendStatus();
}

void FlightControl::rfLevel(int level) {
    qDebug() << Q_FUNC_INFO << level;
}

void FlightControl::messageReceived(QString message) {
    if(message == "pos")
        sendStatus();
}

void FlightControl::startTakeoff() {
    qDebug() << Q_FUNC_INFO;
    messageSendInterval = 0;
    gsmPollTimer.setInterval(1000 * 10);
    gsmPollTimer.start();
    emit pollGsm();
}

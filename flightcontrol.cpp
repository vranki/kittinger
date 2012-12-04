#include "flightcontrol.h"
#include <cmath>
#include <QDebug>

FlightControl::FlightControl(QObject *parent) : QObject(parent) {
    setObjectName("FlightControl");
    connect(&gsmPollTimer, SIGNAL(timeout()), this, SIGNAL(pollGsm()));
    connect(this, SIGNAL(variometerChanged(double)), this, SLOT(checkVario()));
    reset();
}

void FlightControl::reset() {
    lat = lon = alt = NAN;
    gpsStatus = STATUS_NO_FIX;
    messageSendInterval = 0;
    flightState = FS_TAKEOFF;
    groundLevel = highestAltitude = 0;
    variometer = 0;
    lastRfLevel = 0;
    changeState(FS_TAKEOFF);
}

QString FlightControl::stateName(FlightControl::FlightState s) {
    switch(s) {
    case FS_TAKEOFF:
        return "Takeoff";
    case FS_ASCENT:
        return "Ascent";
    case FS_DESCENT:
        return "Descent";
    case FS_LANDED:
        return "Landed";
    case FS_PANIC:
        return "PANIC";
    }
    return "???";
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

    if(alt < groundLevel) groundLevel = alt;
    if(alt > highestAltitude) highestAltitude = alt;

    if(alt != NAN) {
        lastAltFixes.append(alt);
        if(lastAltFixes.size() > 5) {
            lastAltFixes.removeFirst();
            // Recalc variometer value
            variometer = 0;
            for(int i=1;i<5;i++) {
                variometer -= lastAltFixes.value(i-1) - lastAltFixes.value(i);
            }
            emit variometerChanged(variometer);
        }
    }

    if(gsmPollTimer.isActive() && flightState == FS_ASCENT && alt > groundLevel + 1000) {
        gsmPollTimer.stop();
        messageSendInterval = 0;
        log("Stopping GSM polling due to altitude");
    }

    if(!gsmPollTimer.isActive() && flightState == FS_DESCENT && alt < groundLevel + 1000) {
        gsmPollTimer.start();
        messageSendInterval = MESSAGE_INTERVAL;
        log("Starting GSM polling due to altitude");
        emit pollGsm();
    }

    if(messageSendInterval > 0 && QTime::currentTime() > lastMessageSentTime.addSecs(messageSendInterval))
        sendStatus();
}

void FlightControl::rfLevel(int level) {
    lastRfLevel = level;
    if(lastRfLevel == 0) {
        if(flightState == FS_ASCENT) {
            log("GSM signal lost - ending polling");
            gsmPollTimer.stop();
            messageSendInterval = 0;
        }
    } else {
        if(flightState == FS_DESCENT && QTime::currentTime() > lastMessageSentTime.addSecs(messageSendInterval)) {
            sendStatus();
        }
    }
}

void FlightControl::messageReceived(QString message) {
    if(message == "pos")
        sendStatus();
}

void FlightControl::startTakeoff() {
    qDebug() << Q_FUNC_INFO;
    messageSendInterval = 0;
    gsmPollTimer.setInterval(GSM_POLL_INTERVAL);
    gsmPollTimer.start();
    emit pollGsm();
}

void FlightControl::startAscent() {
    qDebug() << Q_FUNC_INFO;
    messageSendInterval = MESSAGE_INTERVAL;
}

void FlightControl::checkVario() {
    switch(flightState) {
    case FS_TAKEOFF:
        if(variometer > VERTICAL_SPEED_DEADZONE) {
            changeState(FS_ASCENT);
            return;
        }
    case FS_ASCENT:
        if(variometer < -VERTICAL_SPEED_DEADZONE) {
            changeState(FS_DESCENT);
            return;
        }
    case FS_DESCENT:
        if(variometer > VERTICAL_SPEED_DEADZONE) { // Wat??
            changeState(FS_ASCENT);
            return;
        }
    }
}

void FlightControl::changeState(FlightControl::FlightState newState) {
    flightState = newState;
    emit flightStateChanged(newState);
    switch(flightState) {
    case FS_TAKEOFF:
        startTakeoff();
        return;
    case FS_ASCENT:
        startAscent();
        return;
    }
}

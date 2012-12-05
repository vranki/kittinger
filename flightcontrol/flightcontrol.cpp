#include "flightcontrol.h"
#include <cmath>
#include <QDebug>

FlightControl::FlightControl(QObject *parent) : QObject(parent) {
    setObjectName("FlightControl");
    connect(&gsmPollTimer, SIGNAL(timeout()), this, SIGNAL(pollGsm()));
    connect(this, SIGNAL(variometerChanged(double)), this, SLOT(checkVario()));
    connect(&detectLandingTimer, SIGNAL(timeout()), this, SLOT(checkForLanding()));
    detectLandingTimer.setInterval(30*1000);
    detectLandingTimer.setSingleShot(false);
    connect(&missionTimeoutTimer, SIGNAL(timeout()), this, SLOT(checkForLanding()));
    missionTimeoutTimer.setInterval(MAX_MISSION_TIME);
    missionTimeoutTimer.setSingleShot(true);
    connect(&sendStatusTimer, SIGNAL(timeout()), this, SLOT(sendStatus()));
    sendStatusTimer.setInterval(MESSAGE_INTERVAL*1000);
    sendStatusTimer.setSingleShot(false);
    reset();
    initialized = false;
}

void FlightControl::reset() {
    lat = lon = alt = NAN;
    gpsStatus = STATUS_NO_FIX;
    flightState = FS_TAKEOFF;
    groundLevel = highestAltitude = 0;
    variometer = 0;
    lastRfLevel = 0;
    landingLevel = 0;
    flightFinished = false;
    initialized = true;
    lastMessageSentTime = QTime::currentTime();
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

void FlightControl::sendStatus(bool force) {
    if(!initialized) return;

    QString status;
    status = QString("%1,%2,%3,%4,%5").arg(lat, 0, 'f', 6).arg(lon, 0, 'f', 6).arg(alt).arg(GPSManager::statusString(gpsStatus),
                                                                                            stateName(flightState).left(1));
    emit sendSms(status);
    log(status);
    lastMessageSentTime = QTime::currentTime();
}

void FlightControl::statusChanged(GpsStatus status) {
    gpsStatus = status;
    emit log("GPS status now " + GPSManager::statusString(gpsStatus));
    if(status == STATUS_NO_FIX && alt < GPS_MAX_ALTITUDE && flightState != FS_TAKEOFF) {
        log("Lost GPS while airborne");
        if(lastRfLevel > 0) sendStatus(true);
    }
}

void FlightControl::gpsFix(double latitude, double longitude, double altitude) {
    lat = latitude;
    lon = longitude;
    alt = altitude;

    if(alt < groundLevel) groundLevel = alt;
    if(alt > highestAltitude) highestAltitude = alt;

    if(alt != NAN) {
        lastAltFixes.append(alt);
        if(lastAltFixes.size() > VARIOMETER_FIXES) {
            lastAltFixes.removeFirst();
            // Recalc variometer value
            variometer = 0;
            for(int i=1;i<VARIOMETER_FIXES;i++) {
                variometer -= lastAltFixes.value(i-1) - lastAltFixes.value(i);
            }
            emit variometerChanged(variometer);
        }
    }

    if(flightState == FS_ASCENT) {
        if(gsmPollTimer.isActive() && alt > groundLevel + GSM_POLL_ALTITUDE) {
            gsmPollTimer.stop();
            sendStatusTimer.stop();
            lastRfLevel = 0;
            log("Stopping GSM polling due to altitude");
        }
    } else if(flightState == FS_DESCENT) {
        if(!gsmPollTimer.isActive() && alt < groundLevel + GSM_POLL_ALTITUDE) {
            gsmPollTimer.start();
            log("Starting GSM polling due to altitude");
            emit pollGsm();
        }
    }
}

void FlightControl::rfLevel(int level) {
    lastRfLevel = level;
    if(lastRfLevel == 0) {
        log("GSM signal lost");
        if(flightState == FS_ASCENT) {
            gsmPollTimer.stop();
            sendStatusTimer.stop();
        } else if(flightState == FS_LANDED && !flightFinished) {
            log("We're on ground with no signal - rebooting");
            reboot();
        }
    } else {
        if(flightState == FS_DESCENT) {
            if(!sendStatusTimer.isActive()) {
                sendStatusTimer.start();
                sendStatus();
            } else { // React asap when going down
                if(lastMessageSentTime.addSecs(MESSAGE_INTERVAL) < QTime::currentTime())
                    sendStatus();
            }
        }
    }
}

void FlightControl::messageReceived(QString message) {
    if(message == "pos") {
        sendStatus();
    } else if(message == "end") {
        flightFinished = true;
        log("end command received - flight has been finished");
        sendStatusTimer.stop();
        missionTimeoutTimer.stop();
        if(flightState != FS_LANDED)
            changeState(FS_LANDED);
    } else if(message == "reboot") {
        reboot();
    } else if(message == "poweroff") {
        poweroff();
    }
}

void FlightControl::startTakeoff() {
    qDebug() << Q_FUNC_INFO;
    gsmPollTimer.setInterval(GSM_POLL_INTERVAL);
    gsmPollTimer.start();
    emit pollGsm();
}

void FlightControl::startAscent() {
    missionStartTime = QTime::currentTime();
    missionTimeoutTimer.start();
    sendStatusTimer.start();
}

void FlightControl::startDescent() {
    if(lastRfLevel > 0) {
        sendStatusTimer.start();
        sendStatus();
    }
}

void FlightControl::startLanded() {
    detectLandingTimer.stop();
    if(lastRfLevel==0) {
        log("We're on ground with no signal - rebooting");
        reboot();
    }
}

void FlightControl::checkVario() {
    switch(flightState) {
    case FS_TAKEOFF:
        if(variometer > VERTICAL_SPEED_DEADZONE) {
            changeState(FS_ASCENT);
            return;
        }
        break;
    case FS_ASCENT:
        if(variometer < -VERTICAL_SPEED_DEADZONE) {
            changeState(FS_DESCENT);
            return;
        }
        break;
    case FS_DESCENT:
        if(variometer > VERTICAL_SPEED_DEADZONE) { // Wat??
            changeState(FS_ASCENT);
            return;
        } else if(variometer==0 && !detectLandingTimer.isActive() && alt < MAX_GROUND_ALTITUDE) {
            landingLevel = alt;
            detectLandingTimer.start();
            qDebug() << Q_FUNC_INFO << "Variometer shows zero - have we landed?";
        }
        break;
    case FS_LANDED:
        break;
    case FS_PANIC:
        break;
    }
}

void FlightControl::checkForLanding() {
    if(gpsStatus == STATUS_NO_FIX) return;
    if(qAbs(alt - landingLevel) < ALTITUDE_ERROR_MARGIN) {
        qDebug() << Q_FUNC_INFO << "Yes, we have landed.";
        changeState(FS_LANDED);
    } else {
        qDebug() << Q_FUNC_INFO << "No, we have not landed. Altitude has changed.";
        detectLandingTimer.stop();
    }
}

void FlightControl::missionTimeout() {
    qDebug() << Q_FUNC_INFO;
    if(!flightFinished && (gpsStatus == STATUS_NO_FIX || lastRfLevel == 0)) {
        log("MISSION TIMEOUT - REBOOTING");
        reboot();
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
    case FS_DESCENT:
        startDescent();
        return;
    case FS_LANDED:
        startLanded();
        return;
    case FS_PANIC:
        break;
    }
}

void FlightControl::reboot() {
    system("echo would now reboot");
}

void FlightControl::poweroff() {
    system("echo would now poweroff");
}

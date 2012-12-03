#ifndef FLIGHTCONTROL_H
#define FLIGHTCONTROL_H

#include <QObject>
#include <QState>
#include <QStateMachine>
#include <QTime>

#include "gpsmanager.h"

class FlightControl : public QObject {
    Q_OBJECT
public:
    explicit FlightControl(QObject *parent = 0);
public slots:
    void sendStatus();
    void statusChanged(GpsStatus status);
    void gpsFix(double latitude, double longitude, double altitude);
    void rfLevel(int level);
    void messageReceived(QString message);
signals:
    void sendSms(QString message);
    void log(QString message);
    void pollGsm();
    void takeOffDetected();
    void descendDetected();
    void landedDetected();

    void survivalDetected();

private slots:
    void startTakeoff();

private:
    double lat, lon, alt;
    GpsStatus gpsStatus;
    QStateMachine machine;
    QState takeoff, ascend, descend, landed, survival;
    QTimer gsmPollTimer;
    QTime lastMessageSentTime;
    int messageSendInterval; // In sec
};

#endif // FLIGHTCONTROL_H

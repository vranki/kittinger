#ifndef FLIGHTCONTROL_H
#define FLIGHTCONTROL_H

#include <QObject>
#include <QState>
#include <QStateMachine>

#include "gpsmanager.h"

class FlightControl : public QObject {
    Q_OBJECT
public:
    explicit FlightControl(QObject *parent = 0);
public slots:
    void sendStatus();
    void statusChanged(GpsStatus status);
    void gpsFix(double latitude, double longitude, double altitude);
signals:
    void sendSms(QString message);
    void log(QString message);

    void takeOffDetected();
    void descendDetected();
    void landedDetected();

    void survivalDetected();

public slots:
private:
    double lat, lon, alt;
    GpsStatus gpsStatus;
    QStateMachine machine;
    QState takeoff, ascend, descend, landed, survival;
};

#endif // FLIGHTCONTROL_H

#ifndef FLIGHTCONTROL_H
#define FLIGHTCONTROL_H

#include <QObject>
#include <QTime>
#include <QQueue>
#include "gpsmanager.h"

#define VARIOMETER_FIXES 5
#define VERTICAL_SPEED_DEADZONE 1
#define MESSAGE_INTERVAL 60 // in sec
#define GSM_POLL_INTERVAL 10*1000 // in msec

class FlightControl : public QObject {
    Q_OBJECT
public:
    enum FlightState {
        FS_TAKEOFF=0,
        FS_ASCENT,
        FS_DESCENT,
        FS_LANDED,
        FS_PANIC
    };

    explicit FlightControl(QObject *parent = 0);
    void reset();
    static QString stateName(FlightControl::FlightState s);
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
    void variometerChanged(double vario);
    void flightStateChanged(FlightControl::FlightState newState);

private slots:
    void startTakeoff();
    void startAscent();
    void checkVario();
private:
    void changeState(FlightState newState);
    double lat, lon, alt;
    double variometer;
    double groundLevel, highestAltitude;
    int lastRfLevel;
    GpsStatus gpsStatus;
    QTimer gsmPollTimer;
    QTime lastMessageSentTime;
    int messageSendInterval; // In sec
    QQueue<double> lastAltFixes;
    FlightState flightState;
};

#endif // FLIGHTCONTROL_H

#ifndef FLIGHTCONTROL_H
#define FLIGHTCONTROL_H

#include <QObject>
#include <QTime>
#include <QQueue>
#include "../gpsmanager.h"
#include "../thermalcontrol.h"

#define VARIOMETER_FIXES 5
#define VERTICAL_SPEED_DEADZONE 1
#define MESSAGE_INTERVAL 20 // in sec
#define GSM_POLL_INTERVAL 10*1000 // in msec
#define GSM_POLL_ALTITUDE 1000 // in m - Altitude above which GSM has no chance of working
#define MAX_MISSION_TIME 4*60*60*1000 // in msec
#define ALTITUDE_ERROR_MARGIN 50 // in m - Amount of gps error allowed to assume landing
#define MAX_GROUND_ALTITUDE 400 // in m - Landing not detected above this
#define GPS_MAX_ALTITUDE 15000 // in m - altitude GPS should be working below

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
    void sendStatus(bool force=false);
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
    void startDescent();
    void startLanded();
    void checkVario();
    void checkForLanding();
    void missionTimeout();

private:
    void changeState(FlightState newState);
    void reboot();
    void poweroff();

    double lat, lon, alt;
    double variometer;
    double groundLevel, highestAltitude, landingLevel;
    int lastRfLevel;
    GpsStatus gpsStatus;
    QTimer gsmPollTimer, sendStatusTimer;
    QTimer detectLandingTimer, missionTimeoutTimer;
    QTime lastMessageSentTime;
    QTime missionStartTime;
    QQueue<double> lastAltFixes;
    FlightState flightState;
    bool initialized; // All initialized
    bool flightFinished; // Confirmed by ground crew

    ThermalControl thermalControl;
};

#endif // FLIGHTCONTROL_H

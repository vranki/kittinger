#include "gpsmanager.h"
#include <math.h>
#include <QDebug>

GPSManager::GPSManager(QObject *parent) : QObject(parent), gps_rec(0) {
    setObjectName("GPSManager");
    connect(&pollTimer, SIGNAL(timeout()), this, SLOT(pollGps()));
    pollTimer.setInterval(10);
    pollTimer.setSingleShot(false);
}

GPSManager::~GPSManager()
{
    delete gps_rec;
}

void GPSManager::openGps()
{
    gps_rec = new gpsmm("localhost", DEFAULT_GPSD_PORT);
    lastStatus = STATUS_NO_FIX;
    lastLatitude = lastLongitude = lastAltitude = 0;

    emit statusChanged(lastStatus);
    emit gpsFix(NAN, NAN, NAN);

    if (!gps_rec->stream(WATCH_ENABLE|WATCH_JSON)) {
        qDebug() << "No GPSD running.";
        return;
    }
    pollTimer.start();
}

QString GPSManager::statusString(GpsStatus status)
{
    switch(status) {
    case STATUS_NO_FIX:
        return "NOFIX";
    case STATUS_SET:
        return "SET";
    case STATUS_DGPS_FIX:
        return "DGPS";
    case STATUS_FIX:
        return "FIX";
    }
    return "?";
}

void GPSManager::pollGps()
{
    if (!gps_rec->waiting(0)) {
        return;
    }
    struct gps_data_t* newdata = gps_rec->read();
    if (!newdata) {
        qDebug() << "Read error";
    } else {
        processData(newdata);
    }
}

void GPSManager::processData(gps_data_t *data)
{
    if(lastStatus != data->status) {
        lastStatus = data->status;
        emit statusChanged(lastStatus);
    }
    if(lastLatitude != data->fix.latitude || lastLongitude != data->fix.longitude || lastAltitude != data->fix.altitude) {
        lastLatitude = data->fix.latitude;
        lastLongitude = data->fix.longitude;
        lastAltitude = data->fix.altitude;
        emit gpsFix(lastLatitude, lastLongitude, lastAltitude);
    }
}

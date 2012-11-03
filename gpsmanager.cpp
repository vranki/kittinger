#include "gpsmanager.h"
#include <math.h>
#include <QDebug>

GPSManager::GPSManager(QObject *parent) : QObject(parent), gps_rec(0) {
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
    emit latitudeChanged(NAN);
    emit longitudeChanged(NAN);
    emit altitudeChanged(NAN);

    if (!gps_rec->stream(WATCH_ENABLE|WATCH_JSON)) {
        qDebug() << "No GPSD running.";
        return;
    }
    pollTimer.start();
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
    if(lastLatitude != data->fix.latitude) {
        lastLatitude = data->fix.latitude;
        emit latitudeChanged(lastLatitude);
    }
    if(lastLongitude != data->fix.longitude) {
        lastLongitude = data->fix.longitude;
        emit longitudeChanged(lastLongitude);
    }
    if(lastAltitude != data->fix.altitude) {
        lastAltitude = data->fix.altitude;
        emit altitudeChanged(lastAltitude);
    }
//    qDebug() << "status " << data->status << "fix " << data->fix.latitude << data->fix.longitude << data->fix.altitude;

}

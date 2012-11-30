#include "gpssimulator.h"

GpsSimulator::GpsSimulator(QObject *parent) :
    QObject(parent)
{
    setObjectName("GPSSimulator");
    enabled = false;
    timer.setSingleShot(false);
    timer.setInterval(1000);
    connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));
    lat = lon = alt = 0;
    status = STATUS_NO_FIX;
}

void GpsSimulator::setLat(double lat)
{
    this->lat = lat;
}

void GpsSimulator::setLon(double lon)
{
    this->lon = lon;
}

void GpsSimulator::setAlt(double alt)
{
    this->alt = alt;
}

void GpsSimulator::setStatus(GpsStatus status)
{
    this->status = status;
    if(enabled)
        emit statusChanged(status);
}

void GpsSimulator::enable(bool enabled)
{
    this->enabled = enabled;
    if(enabled) {
        timer.start();
        emit statusChanged(status);
    } else {
        timer.stop();
    }
}

void GpsSimulator::tick()
{
    emit gpsFix(lat, lon, alt);
}

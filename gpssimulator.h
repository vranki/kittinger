#ifndef GPSSIMULATOR_H
#define GPSSIMULATOR_H

#include <QObject>
#include "gpsmanager.h"
#include <QTimer>

class GpsSimulator : public QObject
{
    Q_OBJECT
public:
    explicit GpsSimulator(QObject *parent = 0);
    void setLat(double lat);
    void setLon(double lon);
    void setAlt(double alt);
    void setStatus(GpsStatus status);
signals:
    void statusChanged(GpsStatus status);
    void latitudeChanged(double lat);
    void longitudeChanged(double lon);
    void altitudeChanged(double alt);
public slots:
    void enable(bool enabled);
private slots:
    void tick();
private:
    bool enabled;
    QTimer timer;
    double lat, lon, alt;
    GpsStatus status;
};

#endif // GPSSIMULATOR_H

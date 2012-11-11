#include <QtGui/QApplication>
#include "mainwindow.h"
#include "gpsmanager.h"
#include "smsmanager.h"
#include "gpssimulator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GpsSimulator gpsSim;

    MainWindow w(0, &gpsSim);

    GPSManager gpsManager;
    w.connect(&gpsManager, SIGNAL(latitudeChanged(double)), &w, SLOT(latitudeChanged(double)));
    w.connect(&gpsManager, SIGNAL(longitudeChanged(double)), &w, SLOT(longitudeChanged(double)));
    w.connect(&gpsManager, SIGNAL(altitudeChanged(double)), &w, SLOT(altitudeChanged(double)));
    w.connect(&gpsManager, SIGNAL(statusChanged(GpsStatus)), &w, SLOT(statusChanged(GpsStatus)));

    SMSManager smsManager;



    w.show();
    gpsManager.openGps();
    smsManager.openCellular();
    smsManager.getRfLevel();
    return a.exec();
}

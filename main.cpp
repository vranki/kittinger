#include <QtGui/QApplication>
#include <QSettings>

#include "mainwindow.h"
#include "gpsmanager.h"
#include "smsmanager.h"
#include "gpssimulator.h"
#include "flightcontrol.h"
#include "settingsdialog.h"
#include "logging.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Kittinger");

    QSettings settings;

    GpsSimulator gpsSim;

    MainWindow w(0, &gpsSim, settings);

    GPSManager gpsManager;
    w.connect(&gpsManager, SIGNAL(gpsFix(double,double,double)), &w, SLOT(gpsFix(double,double,double)));
    w.connect(&gpsManager, SIGNAL(statusChanged(GpsStatus)), &w, SLOT(statusChanged(GpsStatus)));

    SMSManager smsManager;
    w.connect(&smsManager, SIGNAL(rfLevel(int)), &w, SLOT(rfLevelChanged(int)));

    w.show();
    gpsManager.openGps();
    smsManager.openCellular();
    smsManager.getRfLevel();
    smsManager.setSmsNumber(settings.value(SMS_NUMBER_KEY).toString());
    smsManager.setSimulate(settings.value(SIMULATE_KEY).toBool());

    FlightControl fc;
    fc.connect(&fc, SIGNAL(sendSms(QString)), &smsManager, SLOT(sendSms(QString)));
    fc.connect(&w, SIGNAL(sendStatus()), &fc, SLOT(sendStatus()));
    fc.connect(&gpsManager, SIGNAL(gpsFix(double,double,double)), &fc, SLOT(gpsFix(double,double,double)));
    fc.connect(&gpsManager, SIGNAL(statusChanged(GpsStatus)), &fc, SLOT(statusChanged(GpsStatus)));

    fc.connect(&gpsSim, SIGNAL(gpsFix(double,double,double)), &fc, SLOT(gpsFix(double,double,double)));
    fc.connect(&gpsSim, SIGNAL(statusChanged(GpsStatus)), &fc, SLOT(statusChanged(GpsStatus)));

    fc.connect(&fc, SIGNAL(log(QString)), w.log(), SLOT(log(QString)));

    return a.exec();
}

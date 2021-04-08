#include <QApplication>
#include <QSettings>

#include "mainwindow.h"
#include "../gpsmanager.h"
#include "../smsmanager.h"
#include "gpssimulator.h"
#include "../flightcontrol/flightcontrol.h"
#include "settingsdialog.h"
#include "../logging.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Kittinger");

    QSettings settings;

    GpsSimulator gpsSim;
    GPSManager gpsManager;
    SMSManager smsManager;
    FlightControl fc;
    fc.init(&smsManager, &gpsManager);
    MainWindow w(0, &gpsSim, settings, &gpsManager, &smsManager, &fc);
    w.show();
    w.connect(&w, SIGNAL(injectSms(QString)), &smsManager, SLOT(injectSms(QString)));
    w.connect(&fc, SIGNAL(log(QString)), w.log(), SLOT(log(QString)));

    smsManager.setSmsNumber(settings.value(SMS_NUMBER_KEY).toString());
    smsManager.setSimulate(settings.value(SIMULATE_KEY).toBool());

    fc.connect(&w, SIGNAL(simRfLevelChanged(int)), &fc, SLOT(rfLevel(int)));
    fc.connect(&gpsSim, SIGNAL(gpsFix(double,double,double)), &fc, SLOT(gpsFix(double,double,double)));
    fc.connect(&gpsSim, SIGNAL(statusChanged(GpsStatus)), &fc, SLOT(statusChanged(GpsStatus)));
    fc.connect(&fc, SIGNAL(flightStateChanged(FlightControl::FlightState)), &w, SLOT(flightStateChanged(FlightControl::FlightState)));
    fc.connect(&fc, SIGNAL(variometerChanged(double)), &w, SLOT(variometerChanged(double)));

    gpsManager.init();
    smsManager.init();
    fc.reset();
    return a.exec();
}

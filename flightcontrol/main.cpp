#include <QCoreApplication>
#include <QSettings>

#include "../gpsmanager.h"
#include "../smsmanager.h"
#include "flightcontrol.h"
#include "../logging.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    a.setApplicationName("Kittinger");

    QSettings settings;
    Logging log;
    GPSManager gpsManager;
    SMSManager smsManager;

    smsManager.setSmsNumber(settings.value(SMS_NUMBER_KEY).toString());
    smsManager.setSimulate(settings.value(SIMULATE_KEY).toBool());

    FlightControl fc;
    fc.connect(&fc, SIGNAL(sendSms(QString)), &smsManager, SLOT(sendSms(QString)));
    fc.connect(&fc, SIGNAL(pollGsm()), &smsManager, SLOT(pollStatus()));
    fc.connect(&smsManager, SIGNAL(rfLevel(int)), &fc, SLOT(rfLevel(int)));
    fc.connect(&smsManager, SIGNAL(messageReceived(QString)), &fc, SLOT(messageReceived(QString)));
    fc.connect(&gpsManager, SIGNAL(gpsFix(double,double,double)), &fc, SLOT(gpsFix(double,double,double)));
    fc.connect(&gpsManager, SIGNAL(statusChanged(GpsStatus)), &fc, SLOT(statusChanged(GpsStatus)));
    fc.connect(&fc, SIGNAL(log(QString)), &log, SLOT(log(QString)));
    gpsManager.openGps();
    smsManager.openCellular();
    fc.reset();
    return a.exec();
}

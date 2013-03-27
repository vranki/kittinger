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
    fc.init(&smsManager, &gpsManager);
    fc.connect(&fc, SIGNAL(log(QString)), &log, SLOT(log(QString)));
    gpsManager.init();
    smsManager.init();
    fc.reset();
    return a.exec();
}

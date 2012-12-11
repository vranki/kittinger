#include "servocontrol.h"
#include <QDebug>
#include <QString>

ServoControl::ServoControl() : QObject(), minValue(0), maxValue(0) {
}

bool ServoControl::init(int min, int max) {
    if(min <= 0 || min >= max) {
        qDebug() << Q_FUNC_INFO << "Invalid range " << minValue << maxValue;
        return false;
    }
    devFile.setFileName("/dev/servoblaster");
    if(!devFile.exists()) {
        qDebug() << Q_FUNC_INFO << "Servoblaster device file doesn't exist";
        return false;
    }
    if(devFile.isWritable()) {
        qDebug() << Q_FUNC_INFO << "Servoblaster device file not writable";
        return false;
    }
    minValue = min;
    maxValue = max;
    return true;
}

void ServoControl::setServo(int num, float pos) {
    if(!minValue) return; // Not initialized
    if(pos < 0 || pos > 1) {
        qDebug() << Q_FUNC_INFO << "Invalid position " << pos;
        return;
    }
    int posValue = ((float) (maxValue - minValue)) * pos;
    posValue += minValue;
    QString outputString = QString("%1=%2\n").arg(num).arg(posValue);
    if(devFile.open(QIODevice::WriteOnly)) {
        devFile.write(outputString.toUtf8());
        devFile.flush();
        devFile.close();
    } else {
        qDebug() << Q_FUNC_INFO << "Unable to open device file for write!";
    }
}

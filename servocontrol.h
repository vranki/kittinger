#ifndef SERVOCONTROL_H
#define SERVOCONTROL_H

#include <QObject>
#include <QFile>

// Controls servos using Servoblaster kernel module
class ServoControl : public QObject
{
    Q_OBJECT
public:
    explicit ServoControl();
    bool init(int min, int max); // Value range of servo movement. Min must be > 0
    void setServo(int num, float pos); // Servo number and position between 0 and 1.
private:
    int minValue, maxValue;
    QFile devFile;
};

#endif // SERVOCONTROL_H

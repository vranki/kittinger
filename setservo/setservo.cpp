#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <wiringPi.h>
#include <softServo.h>
#include <QDebug>
#include <QString>

/**
  * Setservo - sets a servo to a position.
  *
  * HW: Servo PWM connected to GPIO0
  *     Servo power connected to GPIO1
  *
  * Remember to use voltage converter, not direct connection!
  *
  * 1. Get input value from command line
  * 2. Set pin 1 on to give power to servo
  * 3. Output pwm to pin 0 to drive servo to desired location
  * 4. Turn off pin 1 to cut power
  */

int main (int argc, char *argv[]) {
    if(argc != 2) {
        qDebug() << "Usage: setservo <value 0-1000>";
        return -1;
    }
    QString valueString = argv[1];
    bool ok = false;
    int value = valueString.toInt(&ok);

    if(!ok || value < 0 || value > 1000 ) {
        qDebug() << "Invalid value given";
        return -1;
    }

    if (wiringPiSetup () == -1) {
        qDebug() << "wiringPiSetup failed: " << strerror (errno);
        return 1 ;
    }
    // Turn on servo power
    pinMode (17, OUTPUT);
    digitalWrite (17, true);

    // Setup pwm
    pinMode(4, OUTPUT);
    softServoSetup (-1, -1, -1, -1, value, -1, -1, -1) ;
    softServoWrite (0,  value);

    // Send pwm for a while
    for (int i=0;i<1000;i++)
        delay(10);

    // Turn off power
    digitalWrite (17, false);

    // then quit
    return EXIT_SUCCESS;
}

#ifndef SMSMANAGER_H
#define SMSMANAGER_H

#include <QObject>

class SMSManager : public QObject
{
    Q_OBJECT
public:
    explicit SMSManager(QObject *parent = 0);
    ~SMSManager();
    void openCellular();
    void getRfLevel();
    void sendSms(QString number, QString message, bool simulate);
signals:
    
public slots:
    
private:
    struct gn_statemachine *state;
};

#endif // SMSMANAGER_H

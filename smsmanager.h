#ifndef SMSMANAGER_H
#define SMSMANAGER_H

#include <QObject>
#include <QProcess>
#include <QTimer>
#include <QStringList>

#define SMS_NUMBER_KEY "sms_number"
#define SIMULATE_KEY "sms_simulate"

class SMSManager : public QObject
{
    Q_OBJECT
public:
    explicit SMSManager(QObject *parent = 0);
    ~SMSManager();
    void init();
    void setSmsNumber(QString num);
    void setSimulate(bool sim);
public slots:
    void sendSms(QString message);
    void pollStatus();
    void injectSms(QString message); // As if it is received, for testing
signals:
    void ready(bool isready);
    void rfLevel(int level);
    void smsSendFailed();
    void messageReceived(QString message);
private slots:

private:
    void getMessages();

    QProcess gnokii;
    QString number;
    bool simulate, deviceAvailable;
    QStringList injectedMessages;
};

#endif // SMSMANAGER_H

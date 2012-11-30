#ifndef SMSMANAGER_H
#define SMSMANAGER_H

#include <QObject>
#include <QProcess>
#include <QTimer>

class SMSManager : public QObject
{
    Q_OBJECT
public:
    explicit SMSManager(QObject *parent = 0);
    ~SMSManager();
    void openCellular();
    void getRfLevel();
    void setSmsNumber(QString num);
    void setSimulate(bool sim);
public slots:
    void sendSms(QString message);
signals:
    void ready(bool isready);
    void rfLevel(int level);
    void smsSendFailed();
private slots:
    void gnokiiStarted();
    void gnokiiFinished();
    void readyRead();
private:
    QProcess gnokii;
    QString number;
    bool simulate;
};

#endif // SMSMANAGER_H

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
signals:
    
public slots:
    
};

#endif // SMSMANAGER_H

#include "smsmanager.h"
#include <QDebug>

SMSManager::SMSManager(QObject *parent) : QObject(parent) {
    setObjectName("SMSManager");
    gnokii.setProcessChannelMode(QProcess::MergedChannels);
    simulate = true;
    deviceAvailable = false;
    gotRf = false;
}

SMSManager::~SMSManager() {
    emit ready(false);
    gnokii.write("");
    gnokii.kill();
    gnokii.waitForFinished(5000);
}

void SMSManager::init() {
    gnokii.start("gnokii --identify");
    gnokii.waitForStarted(1000);
    gnokii.waitForFinished(5000);
    while(gnokii.canReadLine()) {
        QString line = gnokii.readLine();
        if(line.startsWith("IMEI")) {
            deviceAvailable = true;
            qDebug() << Q_FUNC_INFO << "Device is available";
        }
    }
    if(!deviceAvailable)
        qDebug() << Q_FUNC_INFO << "Warning: Device is not available";
}

void SMSManager::pollStatus() {
    gnokii.start("gnokii --monitor once\n");
    gnokii.waitForStarted(1000);
    gnokii.waitForFinished(1000);
    bool newMessagesAvailable = false;
    while(gnokii.canReadLine()) {
        QString line = gnokii.readLine();
        if(line.startsWith("RFLevel: ")) {
            QString levelString = line.mid(9);
            qDebug() << Q_FUNC_INFO << "RF Level: " << levelString.trimmed();
            int level = -1;
            bool ok;
            level = levelString.toInt(&ok);
            if(ok) {
                emit rfLevel(level);
                if(level > 0 && !gotRf) {
                    emit ready(true);
                    gotRf = true;
                }
            }
        } else if(line.startsWith("SMS Messages:")) {
            QString numberString = line.mid(line.lastIndexOf(" "));
            int smsms = numberString.toInt();
            newMessagesAvailable = smsms > 1; // For some reason always 1
        }
    }
    if(newMessagesAvailable)
        getMessages();
    foreach(QString msg, injectedMessages)
        emit messageReceived(msg);
    injectedMessages.clear();
}

void SMSManager::injectSms(QString message) {
    injectedMessages.append(message);
}

void SMSManager::getMessages() {
    gnokii.start("gnokii --getsms ME 0");
    gnokii.waitForStarted(1000);
    gnokii.waitForFinished(1000);
    bool readingMessage = false;
    QString message;
    while(gnokii.canReadLine()) {
        QString line = gnokii.readLine();
        if(readingMessage) {
            message += line;
        }
        if(line.startsWith("Text:")) {
            if(!message.isEmpty()) {
                emit messageReceived(message.trimmed().toLower());
                message = "";
                readingMessage = false;
            }
            readingMessage = true;
            message = "";
        }
    }
    if(!message.isEmpty()) {
        emit messageReceived(message.trimmed().toLower());
    }
    gnokii.start("gnokii --deletesms ME 0");
    gnokii.waitForStarted(1000);
    gnokii.waitForFinished(1000);
}

void SMSManager::sendSms(QString message) {
    if(number.isEmpty()) {
        qDebug() << Q_FUNC_INFO << "SMS Number not set! Not sending!";
        return;
    }
    QString cmd = "gnokii --sendsms " + number;
    qDebug() << Q_FUNC_INFO << cmd << message << (simulate ? "SIMULATED" : "Real");
    if(simulate)
        return;
    gnokii.start(cmd);
    gnokii.waitForStarted(1000);
    QByteArray msg = message.toUtf8() + "\n";
    gnokii.write(msg);
    gnokii.closeWriteChannel();
    bool finishedInTime = gnokii.waitForFinished(5000);
    if(!finishedInTime)
        emit smsSendFailed();
    gnokii.kill();
}

void SMSManager::setSmsNumber(QString num)
{
    number = num;
}

void SMSManager::setSimulate(bool sim)
{
    simulate = sim;
}

#include "smsmanager.h"
#include <QDebug>

SMSManager::SMSManager(QObject *parent) : QObject(parent) {
    setObjectName("SMSManager");
    connect(&gnokii, SIGNAL(started()), this, SLOT(gnokiiStarted()));
    connect(&gnokii, SIGNAL(finished(int)), this, SLOT(gnokiiFinished()));
    connect(&gnokii, SIGNAL(readyRead()), this, SLOT(readyRead()));
    gnokii.setProcessChannelMode(QProcess::MergedChannels);
    simulate = true;
}

SMSManager::~SMSManager()
{
    emit ready(true);
    gnokii.write("");
    gnokii.kill();
    gnokii.waitForFinished();
}

void SMSManager::openCellular()
{
    gnokii.start("gnokii --identify");
    gnokii.waitForStarted(1000);
    gnokii.waitForFinished();
}

void SMSManager::getRfLevel() {
    gnokii.start("gnokii --monitor once\n");
    gnokii.waitForStarted(1000);
    gnokii.waitForBytesWritten(100);
    gnokii.waitForFinished();
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

void SMSManager::gnokiiStarted() {
    qDebug() << Q_FUNC_INFO << "Gnokii started ";
    emit ready(true);
}

void SMSManager::gnokiiFinished() {
    qDebug() << Q_FUNC_INFO << "Gnokii finished, code " << gnokii.exitCode() << gnokii.readAllStandardOutput()<< gnokii.readAllStandardError();
}

void SMSManager::readyRead()
{
    if(!gnokii.canReadLine()) return;

    QString line = gnokii.readLine();
    qDebug() << Q_FUNC_INFO << "line:" << line.trimmed();
    if(line.startsWith("RFLevel: ")) {
        QString levelString = line.mid(9);
        qDebug() << Q_FUNC_INFO << "RF Level: " << levelString;
        int level = -1;
        bool ok;
        level = levelString.toInt(&ok);
        if(ok)
            emit rfLevel(level);
    }
}

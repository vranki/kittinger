#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gpssimulator.h"
#include "../smsmanager.h"
#include <QWebFrame>
#include "settingsdialog.h"

MainWindow::MainWindow(QWidget *parent, GpsSimulator *gsim,
                       QSettings &set,
                       GPSManager *gpsManager,
                       SMSManager *smsManager,
                       FlightControl *fc) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), sim(gsim), settings(set) {
    ui->setupUi(this);
    logging.setTextEdit(ui->logTextEdit);
    connect(ui->gpsSimEnable, SIGNAL(toggled(bool)), sim, SLOT(enable(bool)));

    connect(sim, SIGNAL(gpsFix(double,double,double)), this, SLOT(gpsFix(double,double,double)));
    connect(sim, SIGNAL(statusChanged(GpsStatus)), this, SLOT(statusChanged(GpsStatus)));

    connect(ui->latEdit, SIGNAL(textChanged(QString)), this, SLOT(setLat(QString)));
    connect(ui->lonEdit, SIGNAL(textChanged(QString)), this, SLOT(setLon(QString)));
    connect(ui->altEdit, SIGNAL(valueChanged(int)), this, SLOT(setAlt(int)));
    connect(ui->simStatusCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setStatus(int)));

    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(openSettings()));

    connect(ui->sendSmsButton, SIGNAL(clicked()), this, SLOT(sendSms()));

    connect(ui->simRfLevelSpinbox, SIGNAL(valueChanged(int)), this, SIGNAL(simRfLevelChanged(int)));

    connect(gpsManager, SIGNAL(gpsFix(double,double,double)), this, SLOT(gpsFix(double,double,double)));
    connect(gpsManager, SIGNAL(statusChanged(GpsStatus)), this, SLOT(statusChanged(GpsStatus)));
    connect(gpsManager, SIGNAL(ready(bool)), ui->gpsReadyCheckbox, SLOT(setChecked(bool)));

    connect(smsManager, SIGNAL(rfLevel(int)), this, SLOT(rfLevelChanged(int)));
    connect(smsManager, SIGNAL(messageReceived(QString)), log(), SLOT(log(QString)));
    connect(smsManager, SIGNAL(ready(bool)), ui->smsReadyCheckbox, SLOT(setChecked(bool)));

    setLat(ui->latEdit->text());
    setLon(ui->lonEdit->text());
    setAlt(ui->altEdit->value());
}

MainWindow::~MainWindow() {
    delete ui;
}

Logging *MainWindow::log() {
    return &logging;
}

void MainWindow::statusChanged(GpsStatus status) {
    ui->statusCombo->setCurrentIndex((int) status);
}

void MainWindow::gpsFix(double lat, double lon, double alt) {
    ui->latLabel->setText(QString::number(lat));
    ui->lonLabel->setText(QString::number(lon));
    ui->altLabel->setText(QString::number(alt));
    updateMap();
}

void MainWindow::rfLevelChanged(int rfl) {
    ui->rfLevelLabel->setText(QString::number(rfl));
}

void MainWindow::flightStateChanged(FlightControl::FlightState newState) {
    ui->phaseLabel->setText(FlightControl::stateName(newState));
}

void MainWindow::variometerChanged(double vario) {
    ui->variometerValue->setText(QString::number(vario));
}

void MainWindow::setLat(QString lat) {
    sim->setLat(lat.toDouble());
}

void MainWindow::setLon(QString lon) {
    sim->setLon(lon.toDouble());
}

void MainWindow::setAlt(int alt) {
    sim->setAlt(alt);
}

void MainWindow::setStatus(int s) {
    sim->setStatus((GpsStatus) s);
}

void MainWindow::updateMap() {
    QString posString = QString("%1,%2").arg(ui->latLabel->text(), ui->lonLabel->text());
    QString sizeString = QString("%1x%2").arg(ui->mapWebView->width()).arg(ui->mapWebView->height());
    ui->mapWebView->page()->mainFrame()->evaluateJavaScript("showPosition('" + posString + "', '" + sizeString + "'); null");
}

void MainWindow::openSettings() {
    SettingsDialog *sd = new SettingsDialog(this, settings);
    sd->exec();
    sd->deleteLater();
}

void MainWindow::sendSms() {
    emit injectSms(ui->injectMessageEdit->text());
}

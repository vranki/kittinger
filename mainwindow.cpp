#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gpssimulator.h"
#include <QWebFrame>

MainWindow::MainWindow(QWidget *parent, GpsSimulator *gsim) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), logging(ui->logTextEdit), sim(gsim)
{
    ui->setupUi(this);
    connect(ui->gpsSimEnable, SIGNAL(toggled(bool)), sim, SLOT(enable(bool)));

    connect(sim, SIGNAL(latitudeChanged(double)), this, SLOT(latitudeChanged(double)));
    connect(sim, SIGNAL(longitudeChanged(double)), this, SLOT(longitudeChanged(double)));
    connect(sim, SIGNAL(altitudeChanged(double)), this, SLOT(altitudeChanged(double)));
    connect(sim, SIGNAL(statusChanged(GpsStatus)), this, SLOT(statusChanged(GpsStatus)));

    connect(ui->latEdit, SIGNAL(textChanged(QString)), this, SLOT(setLat(QString)));
    connect(ui->lonEdit, SIGNAL(textChanged(QString)), this, SLOT(setLon(QString)));
    connect(ui->altEdit, SIGNAL(textChanged(QString)), this, SLOT(setAlt(QString)));
    connect(ui->simStatusCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setStatus(int)));

    setLat(ui->latEdit->text());
    setLon(ui->lonEdit->text());
    setAlt(ui->altEdit->text());
}

MainWindow::~MainWindow()
{
    delete ui;
}

Logging *MainWindow::log()
{
    return &logging;
}

void MainWindow::statusChanged(GpsStatus status)
{
    ui->statusCombo->setCurrentIndex((int) status);
    // log()->log("Status changed: " + QString::number(status));
}

void MainWindow::latitudeChanged(double lat)
{
    ui->latLabel->setText(QString::number(lat));
    updateMap();
}

void MainWindow::longitudeChanged(double lon)
{
    ui->lonLabel->setText(QString::number(lon));
    updateMap();
}

void MainWindow::altitudeChanged(double alt)
{
    ui->altLabel->setText(QString::number(alt));
}

void MainWindow::setLat(QString lat)
{
    sim->setLat(lat.toDouble());
}

void MainWindow::setLon(QString lon)
{
    sim->setLon(lon.toDouble());
}

void MainWindow::setAlt(QString alt)
{
    sim->setAlt(alt.toDouble());
}

void MainWindow::setStatus(int s)
{
    sim->setStatus((GpsStatus) s);
}

void MainWindow::updateMap()
{
    QString posString = QString("%1,%2").arg(ui->latLabel->text(), ui->lonLabel->text());
    QString sizeString = QString("%1x%2").arg(ui->mapWebView->width()).arg(ui->mapWebView->height());
    ui->mapWebView->page()->mainFrame()->evaluateJavaScript("showPosition('" + posString + "', '" + sizeString + "'); null");
}

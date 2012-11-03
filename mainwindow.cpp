#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::statusChanged(GpsStatus status)
{
    ui->statusCombo->setCurrentIndex((int) status);
}

void MainWindow::latitudeChanged(double lat)
{
    ui->latLabel->setText(QString::number(lat));
}

void MainWindow::longitudeChanged(double lon)
{
    ui->lonLabel->setText(QString::number(lon));
}

void MainWindow::altitudeChanged(double alt)
{
    ui->altLabel->setText(QString::number(alt));
}

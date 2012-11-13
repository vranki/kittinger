#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui/QTextEdit>
#include "gpsmanager.h"
#include "logging.h"

class GpsSimulator;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent, GpsSimulator *gsim);
    ~MainWindow();
    Logging* log();
public slots:
    void statusChanged(GpsStatus status);
    void latitudeChanged(double lat);
    void longitudeChanged(double lon);
    void altitudeChanged(double alt);
private slots:
    void setLat(QString lat);
    void setLon(QString lon);
    void setAlt(QString alt);
    void setStatus(int s);
    void updateMap();
private:
    Ui::MainWindow *ui;
    Logging logging;
    GpsSimulator *sim;
};

#endif // MAINWINDOW_H

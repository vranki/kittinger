#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
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
    explicit MainWindow(QWidget *parent, GpsSimulator *gsim, QSettings &set);
    ~MainWindow();
    Logging* log();
public slots:
    void statusChanged(GpsStatus status);
    void gpsFix(double lat, double lon, double alt);
    void rfLevelChanged(int rfl);
signals:
    void sendStatus();
private slots:
    void setLat(QString lat);
    void setLon(QString lon);
    void setAlt(QString alt);
    void setStatus(int s);
    void updateMap();
    void openSettings();
private:
    Ui::MainWindow *ui;
    Logging logging;
    GpsSimulator *sim;
    QSettings &settings;
};

#endif // MAINWINDOW_H

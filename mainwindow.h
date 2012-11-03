#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gpsmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void statusChanged(GpsStatus status);
    void latitudeChanged(double lat);
    void longitudeChanged(double lon);
    void altitudeChanged(double alt);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

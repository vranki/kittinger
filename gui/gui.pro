#-------------------------------------------------
#
# Project created by QtCreator 2012-11-03T13:42:44
#
#-------------------------------------------------

QT       += core gui webkit

TARGET = kittinger-gui
TEMPLATE = app
CONFIG += link_pkgconfig
PKGCONFIG += libgps

SOURCES += main.cpp\
        mainwindow.cpp \
    ../gpsmanager.cpp \
    ../smsmanager.cpp \
    ../logging.cpp \
    ../thermalcontrol.cpp \
    ../servocontrol.cpp \
    gpssimulator.cpp \
    ../flightcontrol/flightcontrol.cpp \
    settingsdialog.cpp

HEADERS  += mainwindow.h \
    ../gpsmanager.h \
    ../smsmanager.h \
    ../logging.h \
    ../thermalcontrol.h \
    ../servocontrol.h \
    gpssimulator.h \
    ../flightcontrol/flightcontrol.h \
    settingsdialog.h

FORMS    += mainwindow.ui \
    settingsdialog.ui


RESOURCES += ../resources.qrc

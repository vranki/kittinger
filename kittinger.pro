#-------------------------------------------------
#
# Project created by QtCreator 2012-11-03T13:42:44
#
#-------------------------------------------------

QT       += core gui webkit

TARGET = kittinger
TEMPLATE = app
CONFIG += link_pkgconfig
PKGCONFIG += libgps

SOURCES += main.cpp\
        mainwindow.cpp \
    gpsmanager.cpp \
    smsmanager.cpp \
    logging.cpp \
    gpssimulator.cpp \
    flightcontrol.cpp \
    settingsdialog.cpp

HEADERS  += mainwindow.h \
    gpsmanager.h \
    smsmanager.h \
    logging.h \
    gpssimulator.h \
    flightcontrol.h \
    settingsdialog.h

FORMS    += mainwindow.ui \
    settingsdialog.ui

OTHER_FILES += README.txt *.html

RESOURCES += \
    resources.qrc

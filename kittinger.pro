#-------------------------------------------------
#
# Project created by QtCreator 2012-11-03T13:42:44
#
#-------------------------------------------------

QT       += core gui

TARGET = kittinger
TEMPLATE = app
CONFIG += link_pkgconfig
PKGCONFIG += gnokii
PKGCONFIG += libgps

SOURCES += main.cpp\
        mainwindow.cpp \
    gpsmanager.cpp \
    smsmanager.cpp

HEADERS  += mainwindow.h \
    gpsmanager.h \
    smsmanager.h

FORMS    += mainwindow.ui

OTHER_FILES += README.txt

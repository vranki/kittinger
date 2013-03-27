#-------------------------------------------------
#
# Project created by QtCreator 2012-11-03T13:42:44
#
#-------------------------------------------------

QT       += core

TARGET = kittinger
TEMPLATE = app
CONFIG += link_pkgconfig
PKGCONFIG += libgps

SOURCES += main.cpp\
    ../gpsmanager.cpp \
    ../smsmanager.cpp \
    ../logging.cpp \
    ../thermalcontrol.cpp \
    ../servocontrol.cpp \
    flightcontrol.cpp

HEADERS  += ../gpsmanager.h \
    ../smsmanager.h \
    ../logging.h \
    ../thermalcontrol.h \
    ../servocontrol.h \
    flightcontrol.h

QT += core
QT -= gui
QT += serialport
QT += network
QT += sql

QTPLUGIN += QSQLMYSQL
QTPLUGIN += QMYSQL
QTPLUGIN += MYSQL

CONFIG += c++11

TARGET = UART
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    robot.cpp \
    peryferium.cpp \
    parser.cpp \
    mainthread.cpp \
    sensor.cpp \
    actuator.cpp \
    command.cpp \
    battery.cpp \
    sonar.cpp \
    sharp.cpp \
    floor.cpp \
    comport.cpp

HEADERS += \
    robot.h \
    peryferium.h \
    parser.h \
    mainthread.h \
    sensor.h \
    actuator.h \
    command.h \
    battery.h \
    sonar.h \
    sharp.h \
    floor.h \
    comport.h

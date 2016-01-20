QT += core
#QT -= gui
QT += serialport
QT += network
QT += sql

QTPLUGIN += QSQLMYSQL

CONFIG += c++11

TARGET = UART
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app


INCLUDEPATH += C:\opencv_mingw\install\include
LIBS += -L"C:\opencv_mingw\bin"
LIBS += -lopencv_core2411 -lopencv_highgui2411 -lopencv_imgproc2411
#LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_features2d -lopencv_photo -lopencv_ml -lopencv_objdetect

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
    comport.cpp \
    velocity.cpp \
    motor.cpp \
    debuginfo.cpp \
    tcpserver.cpp \
    retransmiter.cpp \
    navigator.cpp \
    state.cpp \
    transition.cpp \
    condition.cpp \
    avoidcollisionalgorithm.cpp \
    camera.cpp \
    servo.cpp \
    imageprocessor.cpp \
    detectionprocess.cpp \
    task.cpp \
    exhaustivesearch.cpp \
    classifier.cpp \
    hogpclassifier.cpp \
    timelimitedstate.cpp

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
    comport.h \
    velocity.h \
    motor.h \
    debuginfo.h \
    retransmiter.h \
    navigator.h \
    state.h \
    transition.h \
    condition.h \
    tcpserver.h \
    motorstate.h \
    avoidcollisionalgorithm.h \
    direction.h \
    enums.h \
    camera.h \
    servo.h \
    imageprocessor.h \
    detectionprocess.h \
    task.h \
    functionalutils.hpp \
    exhaustivesearch.hpp \
    classifier.h \
    hogpclassifier.h \
    timelimitedstate.h

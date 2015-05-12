#-------------------------------------------------
#
# Project created by QtCreator 2015-05-11T22:50:39
#
#-------------------------------------------------

QT       += core gui

QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    fifoThread.cpp \
    ledCmd.cpp \
    ledDevice.cpp

HEADERS  += mainwindow.h \
    fifoThread.hpp \
    ledCmd.hpp \
    ledCommand.hpp \
    ledDevice.hpp

FORMS    += mainwindow.ui

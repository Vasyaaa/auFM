QT += core
QT -= gui
QT += multimedia widgets
CONFIG += c++11

TARGET = aafm
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    aafm.cpp

HEADERS += \
    aafm.h

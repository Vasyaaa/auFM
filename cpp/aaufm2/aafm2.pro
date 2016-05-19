QT += core
QT -= gui
QT += multimedia widgets

CONFIG += c++11

TARGET = aafm2
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    aafm2.cpp

HEADERS += \
    aafm2.h

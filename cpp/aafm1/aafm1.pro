QT += core
QT -= gui
QT += multimedia
QT += multimedia widgets

CONFIG += c++11

TARGET = aafm1
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    aafm1.cpp

HEADERS += \
    aafm1.h

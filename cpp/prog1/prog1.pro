QT += core
QT -= gui
QT += multimedia
QT += multimedia widgets

CONFIG += c++11

TARGET = prog1
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    prog1.cpp


HEADERS += \
    prog1.h

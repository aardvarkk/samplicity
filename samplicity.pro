#-------------------------------------------------
#
# Project created by QtCreator 2014-03-19T15:00:14
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = samplicity
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    database.cpp

HEADERS  += mainwindow.h \
    database.h

FORMS    += mainwindow.ui

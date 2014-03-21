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
    database.cpp \
    command_addfile.cpp \
    command_adddirectory.cpp \
    filesystem.cpp \
    utils.cpp

HEADERS  += mainwindow.h \
    database.h \
    command_addfile.h \
    command_adddirectory.h \
    filesystem.h \
    utils.h

FORMS    += mainwindow.ui

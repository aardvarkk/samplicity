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
    filesystem.cpp \
    utils.cpp \
    directories_model.cpp \
    command_add_directory.cpp \
    command_add_file.cpp

HEADERS  += mainwindow.h \
    database.h \
    filesystem.h \
    utils.h \
    directories_model.h \
    command_add_directory.h \
    command_add_file.h

FORMS    += mainwindow.ui

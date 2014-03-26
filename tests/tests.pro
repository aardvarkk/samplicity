#-------------------------------------------------
#
# Project created by QtCreator 2014-03-26T16:08:25
#
#-------------------------------------------------

QT       += core sql testlib

QT       -= gui

TARGET = samplicity_tests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


HEADERS += ../database.h \
    ../filesystem.h \
    ../sample.h \
    ../utils.h


SOURCES += \
    samplicity_tests.cpp \
    ../database.cpp \
    ../filesystem.cpp \
    ../sample.cpp \
    ../utils.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

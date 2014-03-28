#-------------------------------------------------
#
# Project created by QtCreator 2014-03-19T15:00:14
#
#-------------------------------------------------

QT       += core gui sql multimedia

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
    command_add_file.cpp \
    samples_model.cpp \
    sample.cpp \
    audio_player.cpp \
    settings.cpp \
    edit_tags_dialog.cpp \
    tag.cpp \
    tags_model.cpp

HEADERS  += mainwindow.h \
    database.h \
    filesystem.h \
    utils.h \
    directories_model.h \
    command_add_directory.h \
    command_add_file.h \
    samples_model.h \
    sample.h \
    audio_player.h \
    settings.h \
    edit_tags_dialog.h \
    tag.h \
    tags_model.h

FORMS    += mainwindow.ui \
    edit_tags_dialog.ui

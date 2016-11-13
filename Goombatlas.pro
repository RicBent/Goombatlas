#-------------------------------------------------
#
# Project created by QtCreator 2016-11-05T00:41:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Goombatlas
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mapview.cpp \
    map.cpp \
    mapobjects.cpp \
    nodeeditorwidget.cpp \
    pathbehavioreditor.cpp \
    cstreamwriter.cpp \
    exportmapdialog.cpp \
    mapobjecteditor.cpp \
    spriteseditor.cpp

HEADERS  += mainwindow.h \
    mapview.h \
    map.h \
    mapobjects.h \
    nodeeditorwidget.h \
    pathbehavioreditor.h \
    cstreamwriter.h \
    exportmapdialog.h \
    mapobjecteditor.h \
    misc.h \
    spriteseditor.h

FORMS    += mainwindow.ui \
    exportmapdialog.ui

CONFIG += c++11

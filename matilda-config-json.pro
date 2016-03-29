#-------------------------------------------------
#
# Project created by QtCreator 2016-03-22T12:35:26
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = matilda-config-json
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    energydialog.cpp \
    matildaclient.cpp \
    mysortfilterproxymodel.cpp \
    wait4answerdialog.cpp

HEADERS  += mainwindow.h \
    ../../matilda-bbb/moji_defy.h \
    energydialog.h \
    matildaclient.h \
    mysortfilterproxymodel.h \
    wait4answerdialog.h

FORMS    += mainwindow.ui \
    energydialog.ui \
    wait4answerdialog.ui

RESOURCES += \
    res.qrc

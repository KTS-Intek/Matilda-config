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
    wait4answerdialog.cpp \
    insertmeterdialog.cpp \
    directaccesmatildaservice.cpp \
    selectdevbymacaddrdialog.cpp \
    scanipdialog.cpp \
    udpscanner.cpp \
    treemodel.cpp \
    treeitem.cpp \
    lastdevinfo.cpp \
    settloader.cpp \
    langdialog.cpp \
    showmesshelper.cpp \
    helpform.cpp \
    webtextdocument.cpp

HEADERS  += mainwindow.h \
    ../../matilda-bbb/moji_defy.h \
    energydialog.h \
    matildaclient.h \
    mysortfilterproxymodel.h \
    wait4answerdialog.h \
    insertmeterdialog.h \
    directaccesmatildaservice.h \
    selectdevbymacaddrdialog.h \
    scanipdialog.h \
    udpscanner.h \
    treemodel.h \
    treeitem.h \
    lastdevinfo.h \
    settloader.h \
    langdialog.h \
    showmesshelper.h \
    ucmetereventcodes.h \
    helpform.h \
    webtextdocument.h

FORMS    += mainwindow.ui \
    energydialog.ui \
    wait4answerdialog.ui \
    insertmeterdialog.ui \
    selectdevbymacaddrdialog.ui \
    scanipdialog.ui \
    langdialog.ui \
    helpform.ui

RESOURCES += \
    res.qrc \
    font.qrc \
    html.qrc

TRANSLATIONS = lang_uk.ts\
        lang_en.ts

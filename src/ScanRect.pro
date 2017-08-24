#-------------------------------------------------
#
# Project created by QtCreator 2016-10-29T16:15:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ScanRect
TEMPLATE = app


SOURCES += \
    main.cpp \
    RWScanRect.cpp \
    RWScanProject.cpp \
    RWScanImageEntry.cpp \
    RWScanImageList.cpp \
    RWScanImageModel.cpp

HEADERS  += \
    RWScanRect.h \
    RWScanProject.h \
    RWScanImageEntry.h \
    RWScanImageList.h \
    RWScanImageModel.h

FORMS    += \
    RWScanRect.ui

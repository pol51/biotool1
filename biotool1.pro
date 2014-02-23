TARGET = biotool1
TEMPLATE = app

QT += opengl xml widgets

CONFIG += c++11

GIT_VERSION = $$system("git rev-parse --short HEAD")
DEFINES += BT1_GIT_VERSION=$$GIT_VERSION

QMAKE_CXXFLAGS += -std=gnu++0x

SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/imageView.cpp \
    src/dataCtrl.cpp \
    src/cell.cpp \
    src/settings.cpp \
    src/settingsView.cpp \
    src/csvExportOptions.cpp \
    src/vCil.cpp \
    src/cellItem.cpp \
    src/xpolygon.cpp
HEADERS += src/mainwindow.h \
    src/imageView.h \
    src/dataCtrl.h \
    src/cell.h \
    src/settings.h \
    src/settingsView.h \
    src/csvExportOptions.h \
    src/vCil.h \
    src/cellItem.h \
    src/xpolygon.h

INCLUDEPATH += src

FORMS += ui/mainwindow.ui \
    ui/csvExportOptions.ui

RESOURCES += res/icons.qrc

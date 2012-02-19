TARGET = biotool1
TEMPLATE = app

QT += opengl xml

GIT_VERSION = $$system(git rev-parse --short HEAD)
DEFINES += BT1_GIT_VERSION=$$GIT_VERSION

QMAKE_CXXFLAGS += -std=gnu++0x

SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/imageView.cpp \
    src/dataCtrl.cpp \
    src/cellPolygon.cpp \
    src/cell.cpp \
    src/settings.cpp \
    src/settingsView.cpp
HEADERS += src/mainwindow.h \
    src/imageView.h \
    src/dataCtrl.h \
    src/cellPolygon.h \
    src/cell.h \
    src/settings.h \
    src/settingsView.h

INCLUDEPATH += src

FORMS += ui/mainwindow.ui \
    ui/csvExportOptions.ui

RESOURCES += res/icons.qrc

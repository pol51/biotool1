TARGET = biotool1
TEMPLATE = app
QT += opengl
SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/imageView.cpp
HEADERS += src/mainwindow.h \
    src/imageView.h
INCLUDEPATH += src
FORMS += ui/mainwindow.ui
RESOURCES += res/icons.qrc

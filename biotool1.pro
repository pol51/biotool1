TARGET = biotool1
TEMPLATE = app
QT += opengl
SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/imageView.cpp \
    src/dataCtrl.cpp
HEADERS += src/mainwindow.h \
    src/imageView.h \
    src/dataCtrl.h
INCLUDEPATH += src
FORMS += ui/mainwindow.ui
RESOURCES += res/icons.qrc
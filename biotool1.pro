TARGET = biotool1
TEMPLATE = app
QT += opengl xml
SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/imageView.cpp \
    src/dataCtrl.cpp \
    src/cellPolygon.cpp \
    src/cell.cpp
HEADERS += src/mainwindow.h \
    src/imageView.h \
    src/dataCtrl.h \
    src/cellPolygon.h \
    src/cell.h
INCLUDEPATH += src
FORMS += ui/mainwindow.ui
RESOURCES += res/icons.qrc

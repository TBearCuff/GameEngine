#-------------------------------------------------
#
# Project created by QtCreator 2018-06-10T09:17:25
#
#-------------------------------------------------

QT       += widgets

TARGET = GameEngine
TEMPLATE = lib
CONFIG += staticlib c++11

DESTDIR = ../../Lib
#OBJECTS_DIR = ../../Temp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        GameEngineApp.cpp \
    ../MainWindow/MainWindow.cpp \
    ../MainWindow/Transform3D.cpp \
    BaseGameLogic.cpp

HEADERS += \
        GameEngineApp.h \
    ../MainWindow/MainWindow.h \
    ../MainWindow/Transform3D.h \
    ../MainWindow/vertex.h \
    BaseGameLogic.h \
    interfaces.h

RESOURCES += \
    resources.qrc

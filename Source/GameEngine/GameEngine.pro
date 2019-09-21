#-------------------------------------------------
#
# Project created by QtCreator 2018-06-10T09:17:25
#
#-------------------------------------------------

QT       += widgets xml core gui

TARGET = GameEngine
TEMPLATE = lib
QMAKE_CXXFLAGS += -Wno-literal-suffix -Wno-ignored-qualifiers
CONFIG += staticlib c++11

DESTDIR = ../../Lib


INCLUDEPATH = ../
INCLUDEPATH += ../../3rdParty/zlib

unix {
CONFIG(debug) {
#LIBS += -L$$PWD/../../Lib/LinuxDebug -ltinyxml2d
LIBS += -L$$PWD/../../Lib/LinuxDebug -lz
} else {
#LIBS += -L$$PWD/../../Lib/LinuxRelease -ltinyxml2
LIBS += -L$$PWD/../../Lib/LinuxRelease -lz
}
}

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
    BaseGameLogic.cpp \
    ../MainLoop/Initialization.cpp \
    ../Actor/ActorFactory.cpp \
    GameEngineStd.cpp \
    ../ResourceCache/XMLResourceLoader.cpp \
    ../ResourceCache/ResCache.cpp \
    ../ResourceCache/ZipFile.cpp \
    ../Actor/Actor.cpp \
    ../Graphics3D/Geometry.cpp \
    ../Actor/TransformComponent.cpp \
    ../Utilities/Math.cpp \
    ../MainLoop/Process.cpp \
    ../MainLoop/ProcessManager.cpp \
    ../UserInterface/HumanView.cpp \
    ../Graphics2D/Sprite.cpp \
    ../Graphics3D/Shaders.cpp \
    ../MainWindow/OpenGLRenderWindow.cpp \
    ../Graphics3D/Material.cpp \
    ../Graphics3D/Scene.cpp

HEADERS += \
        GameEngineApp.h \
    ../MainWindow/MainWindow.h \
    ../MainWindow/Transform3D.h \
    ../MainWindow/vertex.h \
    BaseGameLogic.h \
    interfaces.h \
    ../MainLoop/Initialization.h \
    ../Actor/ActorFactory.h \
    GameEngineStd.h \
    ../ResourceCache/XMLResourceLoader.h \
    ../ResourceCache/ResCache.h \
    ../ResourceCache/ZipFile.h \
    ../Actor/Actor.h \
    ../Actor/ActorComponent.h \
    ../Graphics3D/Geometry.h \
    ../Actor/TransformComponent.h \
    ../Utilities/Math.h \
    ../Utilities/templates.h \
    ../MainLoop/Process.h \
    ../MainLoop/ProcessManager.h \
    ../Utilities/types.h \
    ../UserInterface/HumanView.h \
    ../UserInterface/UserInterface.h \
    ../Graphics2D/Sprite.h \
    ../Graphics3D/Shaders.h \
    ../MainWindow/OpenGLRenderWindow.h \
    ../Graphics3D/Material.h \
    ../Graphics3D/Scene.h

RESOURCES += \
    resources.qrc

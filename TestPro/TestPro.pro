TEMPLATE = subdirs

SUBDIRS += \
    ../Source/GameEngine \
    ../../TestGame/Source/TestGame

#build the project sequentially as listed in SUBDIRS (if not default)
CONFIG += ordered

QMAKE_CXXFLAGS += -std=c++0x



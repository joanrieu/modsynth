TEMPLATE = app
QT += quick
LIBS += ../libmodsynth/libmodsynth.a -ljack

QMAKE_CXXFLAGS += -std=c++11
SOURCES = main.cpp

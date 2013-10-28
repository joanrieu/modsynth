TEMPLATE = app
QT += quick
LIBS += ../libmodsynth/libmodsynth.a -ljack

QMAKE_CXXFLAGS += -std=c++11
HEADERS += ModSynth.h
SOURCES += ModSynth.cpp

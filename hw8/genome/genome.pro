TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp

QMAKE_CXXFLAGS += -Wall -Wconversion

CFGPATH = $$PWD/../../lib/cfg

unix:!macx: LIBS += -L$${CFGPATH}/ -lcfg

INCLUDEPATH += $${CFGPATH}/src
DEPENDPATH += $${CFGPATH}/src

unix:!macx: PRE_TARGETDEPS += $${CFGPATH}/libcfg.a

QMAKE_CLEAN += $${TARGET}


TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp

INCLUDEPATH += src/

QMAKE_CXXFLAGS += -Wall -Wconversion

DISTFILES += \
    test/example.gr

CFGPATH = $$PWD/../../lib/cfg

unix:!macx: LIBS += -L$${CFGPATH}/ -lcfg

INCLUDEPATH += $${CFGPATH}/src
DEPENDPATH += $${CFGPATH}/src

unix:!macx: PRE_TARGETDEPS += $${CFGPATH}/libcfg.a

QMAKE_CLEAN += $${TARGET}

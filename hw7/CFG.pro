TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/grammar_parser.cpp \
    src/token.cpp \
    src/main.cpp

HEADERS += \
    src/grammar_parser.h \
    src/token.h

INCLUDEPATH += src/

QMAKE_CXXFLAGS += -Wall -Wconversion

DISTFILES += \
    test/example.gr

TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/grammar_parser.cpp \
    src/token.cpp \
    src/main.cpp \
    src/grammar_converter.cpp \
    src/rules.cpp \
    src/grammar.cpp \
    src/parse_tree.cpp

HEADERS += \
    src/grammar_parser.h \
    src/token.h \
    src/grammar_converter.h \
    src/grammar.h \
    src/rules.h \
    src/parse_tree.h

INCLUDEPATH += src/

QMAKE_CXXFLAGS += -Wall -Wconversion -DPRINT_CSV=0

DISTFILES += \
    test/example.gr

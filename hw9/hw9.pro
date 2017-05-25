TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    src/common.h \
    src/grammar_checker.h \
    src/grammar_converter.h \
    src/grammar_generator.h \
    src/grammar_parser.h \
    src/grammar.h \
    src/graph.h \
    src/parse_tree.h \
    src/rules.h \
    src/token.h

SOURCES += \
    src/grammar_checker.cpp \
    src/grammar_converter.cpp \
    src/grammar_generator.cpp \
    src/grammar_parser.cpp \
    src/grammar.cpp \
    src/graph.cpp \
    src/main.cpp \
    src/parse_tree.cpp \
    src/rules.cpp \
    src/token.cpp


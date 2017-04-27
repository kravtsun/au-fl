#-------------------------------------------------
#
# Project created by QtCreator 2017-04-27T10:18:21
#
#-------------------------------------------------

QT       -= core gui

TARGET = cfg
TEMPLATE = lib
CONFIG += staticlib c++14
#CONFIG += debug

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

unix {
    target.path = /usr/lib
    INSTALLS += target
}

HEADERS += \
    src/grammar.h \
    src/grammar_converter.h \
    src/grammar_parser.h \
    src/parse_tree.h \
    src/rules.h \
    src/token.h \
    src/grammar_checker.h \
    src/graph.h \
    src/common.h \
    src/grammar_generator.h

SOURCES += \
    src/grammar.cpp \
    src/grammar_converter.cpp \
    src/grammar_parser.cpp \
    src/parse_tree.cpp \
    src/rules.cpp \
    src/token.cpp \
    src/grammar_checker.cpp \
    src/graph.cpp \
    src/grammar_generator.cpp

QMAKE_CLEAN += lib$${TARGET}.a

QMAKE_CXXFLAGS += -Wall -Wconversion

QMAKE_CXXFLAGS_RELEASE += -O3
#QMAKE_CXXFLAGS_DEBUG += -O0 -ggdb

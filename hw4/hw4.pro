TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


HEADERS += \
    src/main.y \
    src/op_token.h \
    src/prim_token.h \
    src/simple_token.h \
    src/string_token.h \
    src/token.h \
    src/tokenizer.h

SOURCES += \
    src/op_token.cpp \
    src/lex.yy.cc

LIBS += -lfl

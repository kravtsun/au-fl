TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lfl

HEADERS += \
    src/LGrammar.g \
    src/ast.h \
    src/main.y

DISTFILES += \
    test/arithmetic.L \
    test/example.L \
    test/number.L \
    test/simple.L

SOURCES += \
    src/lex.yy.cpp

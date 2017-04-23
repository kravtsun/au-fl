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

DISTFILES += \
    test/boolean.l \
    test/comments.l \
    test/example.l \
    test/greedy.l \
    test/identifiers.l \
    test/keywords.l \
    test/lonely_star.l \
    test/one_plus_one.l \
    test/rational.l \
    test/specials.l \
    test/split.l \
    test/sum.l \
    test/unknown.l \
    test/mlcomments.l \
    test/greedy_star.l \
    test/multiline.l

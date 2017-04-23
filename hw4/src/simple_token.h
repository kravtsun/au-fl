#ifndef SIMPLE_TOKEN_H
#define SIMPLE_TOKEN_H

#include <string>
#include "token.h"

#define DEAL_SPLIT(Name) \
    struct Name ## Token : public Token \
    {\
        using Token::Token;\
        std::string name() const override { return #Name; };  \
    };

DEAL_SPLIT(LParent);
DEAL_SPLIT(RParent);
DEAL_SPLIT(Colon);

#define DEAL_KEYWORD(Name) \
    struct KW_ ## Name ## Token : public Token \
    {\
        using Token::Token;\
        std::string name() const override { return "KW_" #Name; }\
    };

DEAL_KEYWORD(If);
DEAL_KEYWORD(Then);
DEAL_KEYWORD(Else);
DEAL_KEYWORD(While);
DEAL_KEYWORD(Do);
DEAL_KEYWORD(Read);
DEAL_KEYWORD(Write);
DEAL_KEYWORD(Begin);
DEAL_KEYWORD(End);

#endif // SIMPLE_TOKEN_H


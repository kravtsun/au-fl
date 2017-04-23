#ifndef STRING_TOKEN_H
#define STRING_TOKEN_H

#include <string>
#include "token.h"

#define DEAL_STRINGY_TOKEN(Name)\
struct Name ## Token : public Token\
{\
    Name ## Token(const std::string str, int line, int start, int end)\
        : Token(line, start, end)\
        , str_(str)\
    {}\
\
    std::string name() const override\
    {\
        return #Name;\
    }\
\
    std::string additional() const override\
    {\
        return "\"" + str_ + "\"";\
    }\
\
private:\
    std::string str_;\
};

DEAL_STRINGY_TOKEN(Ident);
DEAL_STRINGY_TOKEN(Comment);
DEAL_STRINGY_TOKEN(Unknown);

#endif // STRING_TOKEN_H


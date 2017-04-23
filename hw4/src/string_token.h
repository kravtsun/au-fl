#ifndef STRING_TOKEN_H
#define STRING_TOKEN_H

#include <algorithm>
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


struct MLCommentToken : public Token
{
    MLCommentToken(const std::string &str
                   , int line_start
                   , int start
                   , int end)
        : Token(line_start, start, end)
        , str_(str)
    {}

    std::string name() const override
    {
        return "MLComment";
    }

    std::string str() const override
    {
        std::ostringstream os;
        std::string str = str_;

        std::replace(str.begin(), str.end(), '\n', ' ');
        os << name() << "(\"" + str + "\", ";
        os << line_;
        os << ", " << start_ << ", " << end_ << ")";
        return os.str();
    }

    MLCommentToken & operator+=(const std::string &s)
    {
        str_ += s;
        end_ += static_cast<int>(s.size());
        return *this;
    }

private:
    std::string str_;
};

#endif // STRING_TOKEN_H


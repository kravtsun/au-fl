#ifndef PRIM_TOKEN_H
#define PRIM_TOKEN_H

#include <string>
#include <cstring>
#include "token.h"

struct NumToken : public Token
{
    NumToken(const std::string &numstr, int line, int start, int end)
        : Token(line, start, end)
    {
        num_ = atof(numstr.c_str());
    }

    std::string name() const override
    {
        return "Num";
    }

    std::string additional() const override
    {
        std::string s = std::to_string(num_);
        s.erase(s.find_last_not_of('0') + 1, std::string::npos);
        if (s.back() == '.')
        {
            s.pop_back();
        }
        return s;
    }

private:
    double num_;
};

struct BoolToken : public Token
{
    BoolToken(const std::string &strvalue, int line, int begin, int end)
        : Token(line, begin, end)
    {
        if (strvalue == "true")
        {
            value_ = true;
        }
        else if (strvalue == "false")
        {
            value_ = false;
        }
        else
        {
            assert(false);
        }
    }

    std::string name() const override
    {
        return "Bool";
    }

    std::string additional() const override
    {
        return value_? "true" : "false";
    }

private:
    bool value_;
};

#endif // PRIM_TOKEN_H


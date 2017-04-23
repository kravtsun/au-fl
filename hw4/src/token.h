#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <cassert>
#include <stdexcept>

struct UnknownTokenException : public std::exception {};

struct Token
{
    Token(int line, int start, int end)
        : line_(line)
        , start_(start)
        , end_(end)
    {}

    virtual std::string name() const
    {
        return "";
    }

    virtual std::string additional() const
    {
        return "";
    }

    // "template" pattern.
    virtual std::string str() const
    {
        std::ostringstream os;
        os << name() << "(";
        const std::string add = additional();
        if (!add.empty())
        {
            os << add << ", ";
        }
        os << line_ << ", " << start_ << ", " << end_ << ")";
        return os.str();
    }

private:
    int line_, start_, end_;
};

#endif 

#ifndef OP_TOKEN_H
#define OP_TOKEN_H

#include <string>
#include <map>
#include "token.h"

enum OpType {
    Plus   ,
    Minus  ,
    Mult   ,
    Divide ,
    Percent,
    Eq     ,
    Neg    ,
    Gt     ,
    Ge     ,
    Lt     ,
    Le     ,
    And    ,
    Or     ,
    Pow    ,
};

struct OpToken : public Token 
{

    static std::map<std::string, OpType> strop;
    static std::map<OpType, std::string> opname;
    static std::map<OpType, std::string> opstr;

    OpToken(const std::string &str, int line, int start, int end)
        : Token(line, start, end)
    {
        if (strop.find(str) == strop.end())
        {
            throw UnknownTokenException();
        }
        type_ = strop.at(str);
    }

    std::string name() const override
    {
        return "Op";
    }
    
    std::string additional() const override
    {
        return opname[type_];
    }

private:
    OpType type_;
};

#endif // OP_TOKEN_H


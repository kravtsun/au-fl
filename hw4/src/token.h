#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <cassert>

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

    // "template" pattern?
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
    };
struct OpToken : public Token 
{

    static std::map<std::string, OpType> strop;
    static std::map<OpType, std::string> opname;
    static std::map<OpType, std::string> opstr;

    OpToken(const std::string &str, int line, int start, int end)
        : Token(line, start, end)
    {
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

#define DEAL_SPLIT(Name) \
    struct Name ## Token : public Token \
    {\
        using Token::Token;\
        std::string name() const override { return #Name; };  \
    };

DEAL_SPLIT(LParent);
DEAL_SPLIT(RParent);
DEAL_SPLIT(Colon);

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

#endif 

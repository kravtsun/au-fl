#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <cassert>
#include "token.h"

template<typename T>
void debug(const T &x, const std::string &name="")
{
    std::cout << name << x << std::endl;
}

#define DEAL_STRINGY(Name) \
    else if (name == #Name)\
    {\
        tokens_.emplace_back(new Name ## Token (str, line, begin, end));\
    }

#define TOKENIZER_DEAL_SIMPLE(Name) \
    else if (name == #Name) \
    {\
        tokens_.emplace_back(new Name ## Token (line, begin, end));\
    }

#define TOKENIZER_DEAL_KEYWORD(Name) \
    else if (name == #Name)\
    {\
        tokens_.emplace_back(new KW_ ## Name ## Token(line, begin, end));\
    }

struct Tokenizer {

    void emplace(const std::string &name, const std::string &str, const std::string &prefix,
                 int line, int begin, int end)
    {
        // debug(name, "name=");
        // debug(str, "str=");
        // debug(prefix, "prefix=");
        // std::cout << std::endl;

        if (false);
        DEAL_STRINGY(Comment)
        DEAL_STRINGY(Bool)
        DEAL_STRINGY(Ident)
        DEAL_STRINGY(Op)
        DEAL_STRINGY(Num)
        DEAL_STRINGY(Unknown)
        TOKENIZER_DEAL_SIMPLE(LParent)
        TOKENIZER_DEAL_SIMPLE(RParent)
        TOKENIZER_DEAL_SIMPLE(Colon)
        else if (prefix == "KW_")
        {
            if (false);
            TOKENIZER_DEAL_KEYWORD(If)
            TOKENIZER_DEAL_KEYWORD(Then)
            TOKENIZER_DEAL_KEYWORD(Else)
            TOKENIZER_DEAL_KEYWORD(While)
            TOKENIZER_DEAL_KEYWORD(Do)
            TOKENIZER_DEAL_KEYWORD(Read)
            TOKENIZER_DEAL_KEYWORD(Write)
            TOKENIZER_DEAL_KEYWORD(Begin)
            TOKENIZER_DEAL_KEYWORD(End)
        }
        else
        {
            assert(false);
        }
    }

    void print()
    {
        std::for_each(tokens_.cbegin(), tokens_.cend(), [](std::shared_ptr<Token> p) { std::cout << p->str() << "; "; });
        // std::cout << std::endl;
    }
    
private:
    std::vector<std::shared_ptr<Token>> tokens_;
};


#endif // TOKENIZER_H

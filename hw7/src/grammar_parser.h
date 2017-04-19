#ifndef GRAMMARPARSER_H
#define GRAMMARPARSER_H

#include <sstream>
#include <algorithm>
#include <iterator>
#include <cassert>

#include "token.h"

class GrammarParser
{
public:
    GrammarParser(std::istream &is);

    bool operator()();

    Rule parse_rule();

    std::istream &is_;
};

#endif // GRAMMARPARSER_H

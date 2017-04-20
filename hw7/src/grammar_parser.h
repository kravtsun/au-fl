#ifndef GRAMMARPARSER_H
#define GRAMMARPARSER_H

#include <sstream>
#include <algorithm>
#include <iterator>
#include <cassert>

#include "rules.h"

class GrammarParser
{
public:
    GrammarParser(std::istream &is);

    operator bool() const;

    Rules parse_line();

    std::istream &is_;
};

#endif // GRAMMARPARSER_H

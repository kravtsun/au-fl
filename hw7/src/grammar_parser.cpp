#include "grammar_parser.h"

GrammarParser::GrammarParser(std::istream &is)
    : is_(is)
{}

bool GrammarParser::operator()() {
    return static_cast<bool>(is_);
}

Rule GrammarParser::parse_rule() {
    std::string line;
    std::getline(is_, line);
    if (line.empty()) {
        std::getline(is_, line);
    }
    if (line.empty() || !is_) {
        return Rule::empty;
    }
    std::istringstream isline(line);
    std::string name;
    isline >> name;
    char c;
    isline >> c;
    assert(c == '=');

    std::vector<Alternative> alternatives;
    std::copy(std::istream_iterator<Alternative>(isline), std::istream_iterator<Alternative>(), std::back_inserter(alternatives));
    return Rule(name, alternatives);
}

#include "grammar_parser.h"
#include <iterator>
#include <sstream>

GrammarParser::GrammarParser(std::istream &is)
    : is_(is)
{}

GrammarParser::operator bool() const {
    return static_cast<bool>(is_);
}

Rules GrammarParser::parse_line() {
    std::string line;
    std::getline(is_, line);
    if (line.empty()) {
        std::getline(is_, line);
    }
    if (line.empty() || !is_) {
        return {};
    }

    std::istringstream isline(line);
    std::string name;
    isline >> name;
    char c;
    isline >> c;
    assert(c == '=');

    Rules rules;
    auto left = TokenFactory::factory(name);
    assert(left && left->isNonTerminal());
    do {
        Alternative a;
        isline >> a;
        rules.emplace_back(left, a);
    } while (isline);
    assert(std::all_of(all(rules), [](const Rule &r) -> bool { return r.left() && static_cast<bool>(*r.left()); }));
    return rules;
}

#include <set>
#include <list>
#include "grammar_converter.h"

GrammarConverter::GrammarConverter(const std::__cxx11::string &start_name, const Rules &rules)
    : start_name_(start_name)
    , rules_(rules)
    , namer_(rules)
{}

bool GrammarConverter::isNormal() {
    int eps_cnt = 0;
    for (auto const &r : rules_) {
        assert(r.left().isNonTerminal());
        assert(!r.right().empty());

        if (r.right().size() > 2) return false;
        else if (r.right().size() == 1) {
            auto const &t = *r.right().back();
            if (t.isEpsilon()) {
                eps_cnt++;
            }
            else if (t.isNonTerminal()) {
                return false;
            }
        }
        else {
            if (!r.right().front()->isNonTerminal() || !r.right().back()->isNonTerminal()) {
                return false;
            }
        }
    }
    return eps_cnt <= 1;
}


void GrammarConverter::add_start() {
    bool has_start_in_right = false;
    for (auto const &r : rules_) {
        auto it = std::find_if(all(r.right()), [this](TokenType t) -> bool { return t->isNonTerminal() && t->str() == start_name_; });
        has_start_in_right = has_start_in_right || it == r.right().end();
    }
    if (!has_start_in_right) return;

    std::string new_start_name = namer_.next_name();
    auto new_start = TokenFactory::factory(new_start_name);
    assert(new_start);
    auto old_start = TokenFactory::factory(start_name_);
    assert(old_start->isNonTerminal());
    Alternative new_alternative(1, old_start);
    Rule new_rule(new_start, new_alternative);
    rules_.emplace_back(new_rule);
    start_name_ = new_start_name;
}

void GrammarConverter::convertToChomsky()
{
    add_start();
}

#include <set>
#include <list>
#include "grammar_converter.h"

GrammarConverter::GrammarConverter(const std::string &start_name, const Rules &rules)
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

void GrammarConverter::remove_long_productions()
{
    for (auto it = rules_.begin(); it != rules_.end(); ++it) {
        if (it->right().size() > 2) {
            auto &a = it->right();
            const std::string cur_name = it->left().str();
//             TODO: replace with it->left() // (need to change Rule's interface to deal with left as TokenType).
            auto cur_non_terminal = TokenFactory::factory(cur_name);
//            auto cur_non_terminal = it->left();
            for (size_t i = 0; i+2 < a.size(); ++i) {
                const std::string new_name = namer_.next_name();
                auto new_non_terminal = TokenFactory::factory(new_name);
                assert(new_non_terminal->isNonTerminal());
                const Alternative new_alternative = {a[i], new_non_terminal};
                rules_.emplace_back(cur_non_terminal, new_alternative);
                std::swap(cur_non_terminal, new_non_terminal);
            }
            Alternative new_alternative = {a[a.size()-2], a[a.size()-1]};
            rules_.emplace_back(cur_non_terminal, new_alternative);
            rules_.erase(it);
        }
        assert(rules_.size() < 1000);
    }
}

void GrammarConverter::remove_null_productions()
{

}


void GrammarConverter::convertToChomsky()
{
    add_start();
    remove_long_productions();
    remove_null_productions();
}

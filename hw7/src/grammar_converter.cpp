#include <set>
#include <list>
#include <iterator>
#include <iostream>
#include "grammar_converter.h"
#include <queue>
#include <map>

GrammarConverter::GrammarConverter(const std::string &start_name, const Rules &rules)
    : start_name_(start_name)
    , rules_(rules)
    , namer_(rules)
{}

bool GrammarConverter::isNormal() {
    int eps_cnt = 0;
    for (auto const &r : rules_) {
        assert(r.left()->isNonTerminal());
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
        auto it =   std::find_if(all(r.right()), [this](TokenType t) -> bool { return t->isNonTerminal() && t->str() == start_name_; });
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
            auto cur_non_terminal = it->left();
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
    std::map<TokenType, bool, TokenFactory::TokenTypeComp> iseps;
    auto isEpsilonToken = [&iseps](TokenType t) -> bool { return t->isEpsilon() || (t->isTerminal() && iseps[t]); };
    std::queue<TokenType> q;
    do {
        if (!q.empty()) {
            q.pop();
        }
        for (auto const &r : rules_) {
            if (!iseps[r.left()] && (r.left()->isEpsilon() || std::all_of(all(r.right()), isEpsilonToken))) {
                iseps[r.left()] = true;
                q.push(r.left());
            }
        }
    } while (!q.empty());

    std::vector<std::string> v;
    for (auto const &p : iseps) {
        if (p.second) {
            v.push_back(p.first->str());
        }
    }
    std::copy(all(v), std::ostream_iterator<std::string>(std::cout, ", "));
}


void GrammarConverter::convertToChomsky()
{
    add_start();
    remove_long_productions();
    remove_null_productions();
}

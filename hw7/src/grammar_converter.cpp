#include <set>
#include <list>
#include <iterator>
#include <iostream>
#include <queue>
#include <map>
#include <cassert>

template<typename T> void debug(const T &x) {
    std::cout << "[DEBUG] " << x << std::endl;
}

#include "grammar_converter.h"

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

    std::string new_start_name = namer_.next_name(start_name_);
    auto new_start = TokenFactory::factory(new_start_name);
    assert(new_start);
    auto old_start = TokenFactory::factory(start_name_);
    assert(old_start->isNonTerminal());
    Alternative new_alternative(1, old_start);
    assert(new_start);
    Rule new_rule(new_start, new_alternative);
    rules_.emplace_back(new_rule);
    start_name_ = new_start_name;
}

void GrammarConverter::after_party()
{
    std::vector<Rule> sorted_rules(all(rules_));
    std::sort(all(sorted_rules));
    rules_.assign(all(sorted_rules));
}

void GrammarConverter::remove_long_productions()
{
    for (auto it = rules_.begin(); it != rules_.end(); ) {
        if (it->right().size() > 2) {
            auto &a = it->right();
            auto cur_non_terminal = it->left();
            for (size_t i = 0; i+2 < a.size(); ++i) {
                const std::string new_name = namer_.next_name(it->left()->str());
                auto new_non_terminal = TokenFactory::factory(new_name);
                assert(new_non_terminal && new_non_terminal->isNonTerminal());
                const Alternative new_alternative = {a[i], new_non_terminal};
                rules_.emplace_back(cur_non_terminal, new_alternative);
                std::swap(cur_non_terminal, new_non_terminal);
            }
            Alternative new_alternative = {a[a.size()-2], a[a.size()-1]};
            assert(cur_non_terminal);
            rules_.emplace_back(cur_non_terminal, new_alternative);
            it = rules_.erase(it);
        }
        else {
            it++;
        }
        assert(rules_.size() < 1000);
    }
    after_party();
}

void GrammarConverter::remove_null_productions()
{
    std::map<TokenType, bool, TokenFactory::TokenTypeComp> iseps;
    auto isEpsilonToken = [&iseps](TokenType t) -> bool { return t->isEpsilon() || (t->isNonTerminal() && iseps[t]); };
    std::queue<TokenType> q;
    do {
        if (!q.empty()) {
            q.pop();
        }
        for (auto const &r : rules_) {
            if (!iseps[r.left()] && std::all_of(all(r.right()), isEpsilonToken)) {
                iseps[r.left()] = true;
                q.push(r.left());
            }
        }
    } while (!q.empty());

    if (std::count_if(all(iseps), [](const std::pair<TokenType, bool> &p) { return p.second; }) == 0) return;

    auto eps_token = TokenFactory::factory(EPSILON);
    auto new_start_token = TokenFactory::factory(namer_.next_name(start_name_));
    Alternative eps_alternative = {eps_token};
    rules_.emplace_back(new_start_token, eps_alternative);
    Alternative cur_start_alternative = {TokenFactory::factory(start_name_)};
    rules_.emplace_back(new_start_token, cur_start_alternative);
    start_name_ = new_start_token->str();

    for (auto it = rules_.begin(); it != rules_.end(); ) {
        assert(it->left() != nullptr);
        auto const &v = it->right();
        if (std::any_of(all(v), isEpsilonToken)) {
            if (v.size() == 2) {
                Alternative first_alternative = {v[0]},
                        second_alternative = {v[1]};
                assert(it->left());
                if (isEpsilonToken(v[0])) {
                    rules_.emplace_back(it->left(), second_alternative);
                }
                if (isEpsilonToken(v[1])) {
                    rules_.emplace_back(it->left(), first_alternative);
                }
            }
            else {
                assert(v.size() == 1);
            }
        }

        if (it->left()->str() != start_name_ && v.size() == 1 && v.front()->isEpsilon()) {
            it = rules_.erase(it);
        }
        else {
            it++;
        }
//            it = rules_.erase(it);
//        }
//        else {
//            it++;
//        }
        assert(rules_.size() < 1000);
    }

    after_party();

//    std::vector<std::string> v;
//    for (auto const &p : iseps) {
//        if (p.second) {
//            v.push_back(p.first->str());
//        }
//    }
    //    std::copy(all(v), std::ostream_iterator<std::string>(std::cout, ", "));
}

void GrammarConverter::remove_circuit_rules()
{
    auto isCurcuitRule = [](const Rule &r) -> bool { return r.right().size() == 1 && r.right()[0]->isNonTerminal(); };

    std::map<TokenType, std::vector<Alternative>, TokenTypeComp> token_alternatives;
//    std::map<TokenType, std::vector<Rules::iterator>, TokenTypeComp> token_noncircuit;
    std::vector<Rules::iterator> circuit_rules;
    std::map<TokenType, std::set<TokenType, TokenTypeComp>, TokenTypeComp> circuit_pairs;
    std::queue<TokenType> q;

    for (auto it = rules_.begin(); it != rules_.end(); ++it) {
        auto t = it->left();
        if (isCurcuitRule(*it)) {
//            circuit_pairs[t].insert(t);
            assert(it->right().size() == 1);
            circuit_pairs[t].insert(it->right().front());
            q.push(t);
            circuit_rules.push_back(it);
        }
        else {
            token_alternatives[t].push_back(it->right());
        }
//        else {
//            token_noncircuit[it->left()].push_back(it);
//        }
    }
    std::set<std::pair<TokenType, TokenType>, TokenTypePairComp> worked_pairs;

    while (!q.empty()) {
        auto t = q.front();
        q.pop();
//        debug("t = " + t->str());
        std::set<TokenType, TokenTypeComp> new_to;
        for (auto const &t2 : circuit_pairs[t]) {
//            debug("t2 = " + t2->str());
            auto p = std::make_pair(t, t2);
            for (auto const &t3 : circuit_pairs[t2]) {
                if (circuit_pairs.at(t).count(t3) == 0) {
                    new_to.insert(t3);
                }
            }
            if (t == t2 || worked_pairs.count(p)) continue;
            worked_pairs.insert(p);
            for (auto const &a : token_alternatives[t2]) {
                rules_.emplace_back(t, a);
            }
        }
        if (!new_to.empty()) {
            for (auto const & t3 : new_to) {
                circuit_pairs.at(t).insert(t3);
            }
            q.push(t);
        }
        assert(q.size() < 1000);
    }

    for (auto const &it : circuit_rules) {
        rules_.erase(it);
    }

    after_party();
}

void GrammarConverter::remove_non_generating()
{
    std::set<TokenType, TokenTypeComp> generating;
    for (auto const &r : rules_) {
        bool no_non_terminals = std::none_of(all(r.right()),
              [](const TokenType &t) -> bool { return t->isNonTerminal(); });
        if (no_non_terminals) {
            generating.insert(r.left());
        }
    }

    const auto isGoodRule = [&generating](const Rule &r) -> bool {
        for (auto const &t : r.right()) {
            if (t->isNonTerminal() && generating.count(t) == 0) {
                return false;
            }
        }
        return true;
    };

    bool fl = true;
    while (fl) {
        fl = false;
        const size_t old_generating_size = generating.size();
        for (auto const &r : rules_) {
            if (isGoodRule(r)) {
                generating.insert(r.left());
            }
        }

        assert(old_generating_size <= generating.size());
        fl = old_generating_size < generating.size();
    }

    const auto isNonGenerating = [&generating](const TokenType &t) -> bool { return t->isNonTerminal() && generating.count(t) == 0; };
    for (auto it = rules_.begin(); it != rules_.end(); ) {
        bool isForRemoval = isNonGenerating(it->left());
        isForRemoval = isForRemoval || std::any_of(all(it->right()), isNonGenerating);
        if (isForRemoval) {
            it = rules_.erase(it);
        }
        else {
            it++;
        }
    }

    after_party();
}



void GrammarConverter::remove_terminal_only()
{
    for (auto it = rules_.begin(); it != rules_.end(); ) {
        auto &a = it->right();
        bool forRemoval = false;
        Alternative alternative;
        if (a.size() > 1) for (auto &t : a) {
            if (t->isTerminal()) {
                auto new_non_terminal = TokenFactory::factory(namer_.next_name(it->left()->str()));
                Alternative new_alternative = {t};
                rules_.emplace_back(new_non_terminal, new_alternative);
//                t = new_non_terminal;
                forRemoval = true;
                alternative.push_back(new_non_terminal);
            }
            else {
                alternative.push_back(t);
            }
        }

        if (forRemoval) {
            rules_.emplace_back(it->left(), alternative);
            it = rules_.erase(it);
        }
        else {
            it++;
        }
    }

    after_party();
}

void GrammarConverter::convertToChomsky()
{
//    add_start();
    remove_long_productions();
    remove_null_productions();
    remove_circuit_rules();
    remove_non_generating();
    remove_terminal_only();
}

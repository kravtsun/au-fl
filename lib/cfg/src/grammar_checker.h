#ifndef GRAMMAR_CHECKER_H
#define GRAMMAR_CHECKER_H

#include <unordered_set>
#include <set>
#include <vector>
#include <iterator>
#include <algorithm>
#include <queue>
#include "grammar.h"
#include "parse_tree.h"
#include "graph.h"

class GrammarChecker {
public:
    GrammarChecker(const Grammar &gr);

    ParseTree build_tree();

    bool check(const std::string &word);

    void show_table(bool print_csv=false);

private:
    int istart() const {
        return token_map_.at(start_token_);
    }

    int non_terminal_index(const TokenType &t) {
        assert(t->isNonTerminal());
        return token_map_.at(t);
    }

private:
    // Non terminals.
    TokenType start_token_;
    std::map<TokenType, int, TokenFactory::TokenTypeComp> token_map_;
    std::vector<TokenType> non_terminals_;

    // Rules
    std::unordered_map<char, vint> symbol_rules_;
    std::unordered_map<token_t, vptt> pair_rules_;
    bool has_epsilon_;

    // Cache.
    std::string word_;
    vvvbool dp_;

    friend class GrammarGenerator;
};

#endif // GRAMMAR_CHECKER_H

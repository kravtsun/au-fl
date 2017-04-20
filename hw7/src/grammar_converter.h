#ifndef GRAMMARCONVERTER_H
#define GRAMMARCONVERTER_H
#include <string>
#include <set>
#include "rules.h"

struct GrammarConverter
{
    using TokenTypeComp = TokenFactory::TokenTypeComp;
    using TokenTypePairComp = TokenFactory::TokenTypePairComp;
    GrammarConverter(const std::string &start_name, const Rules &rules);

    void convertToChomsky();

    bool isNormal();

    const Rules &rules() const {
        return rules_;
    }

private:
    std::string start_name_;
    Rules rules_;

    struct Namer {
        Namer(const Rules &rules)
            : names_(all_non_terminals_names(rules))
            , current_name_("A")
        {}

        std::string next_name(const std::string &base) {
#if 0
            bool fl = false;
            for (auto it = current_name_.rbegin(); it != current_name_.rend(); it++) {
                if (*it != 'Z') {
                    (*it)++;
                    fl = true;
                }
            }
            if (!fl) {
                current_name_.assign(current_name_.size() + 1, 'A');
            }
            if (names_.count(current_name_) == 0) {
                names_.insert(current_name_);
                return current_name_;
            }
#else
            for (int i = 1; ; ++i) {
                std::string s = base + std::to_string(i);
                if (names_.count(s) == 0) {
                    names_.insert(s);
                    return s;
                }
            }
#endif
            return next_name(base);
        }

    private:
        std::set<std::string> names_;
        std::string current_name_;

        static std::set<std::string> all_non_terminals_names(const Rules &rules) {
            std::set<std::string> res;
            for (auto const &r : rules) {
                TokenType t = r.left();
                assert(t && (bool)(*t));
                res.insert(t->str());
                for (auto const & t : r.right()) {
                    if (t->isNonTerminal()) {
                        res.insert(t->str());
                    }
                }
            }
            return res;
        }
    } namer_;

    void add_start();

    void remove_long_productions();

    void remove_null_productions();

    void remove_circuit_rules();

    void remove_non_generating();

    void remove_not_reachable();

    void after_party();

    // TODO.
//    void remove_duplicates();

    void remove_terminal_only();
};

#endif // GRAMMARCONVERTER_H

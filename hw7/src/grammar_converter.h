#ifndef GRAMMARCONVERTER_H
#define GRAMMARCONVERTER_H
#include <string>
#include <set>
#include "rules.h"

struct GrammarConverter
{
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

        std::string next_name() {
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
                return current_name_;
            }
            return next_name();
        }

    private:
        std::set<std::string> names_;
        std::string current_name_;

        static std::set<std::string> all_non_terminals_names(const Rules &rules) {
            std::set<std::string> res;
            for (auto const &r : rules) {
                res.insert(r.left().str());
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
};

#endif // GRAMMARCONVERTER_H

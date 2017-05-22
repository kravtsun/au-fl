#ifndef GRAMMARCONVERTER_H
#define GRAMMARCONVERTER_H
#include <string>
#include <set>
#include "rules.h"

class GrammarConverter
{
    using TokenTypeComp = TokenFactory::TokenTypeComp;
    using TokenTypePairComp = TokenFactory::TokenTypePairComp;
public:
    GrammarConverter(const std::string &start_name, const Rules &rules);

    void convertToChomsky();

    const Rules &rules() const;

private:
    std::string start_name_;
    Rules rules_;

    struct Namer {
        Namer(const Rules &rules);

        std::string next_name(const std::string &base);

    private:
        std::set<std::string> names_;
        std::string current_name_;

        static std::set<std::string> all_non_terminals_names(const Rules &rules);
    } namer_;

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

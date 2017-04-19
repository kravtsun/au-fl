#include <iostream>
#include <list>
#include <algorithm>
#include "token.h"
#include "grammar_parser.h"

#define forn(i, n) for (int i = 0; i < (int)(n); ++i)
#define all(x) (x).begin(), (x).end()

using namespace std;

void print_rules(std::vector<Rule> &rules) {
    int cnt = 0;
    for (auto const &r : rules) {
        cout << "Rule #" << cnt++ << ": " << r.left() << " = ";
        auto const &alternatives = r.alternatives();
        int level = 0;
        for (auto const &a : alternatives) {
            forn(i, level) {
                cout << " ";
            }
            level += 4;

            std::cout << a << " | \n";
        }
//        std::copy(all(alternatives), std::ostream_iterator<Alternative>(std::cout, "\n"));
    }
}


Rule merge_rules(const Rule &lhs, const Rule &rhs) {
    assert(lhs.left() == rhs.left());
    Rule::alternatives_type all_alternatives;
    std::merge(all(lhs.alternatives()), all(rhs.alternatives()), std::back_inserter(all_alternatives));
    return Rule(lhs.left(), all_alternatives);
}

void merge_rules(std::vector<Rule> &rules) {
    std::sort(all(rules), [](const Rule &lhs, const Rule &rhs) -> bool { return lhs.left().str() < rhs.left().str(); });
    std::list<Rule> lrules(all(rules));

    for (auto it = lrules.begin(); it != lrules.end(); it++) {
        auto next = it;
        next++;
        while (next != lrules.end()) {
            if (next->left() != it->left()) break;
            *it = merge_rules(*it, *next);
            lrules.erase(next);
            next = it;
            next++;
        }
    }
    rules.assign(all(lrules));
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    GrammarParser gp(cin);

    NonTerminal start;
    cin >> start;

    Rule r;
    std::vector<Rule> rules;
    while (r = gp.parse_rule()) {
        rules.push_back(r);
    }

    merge_rules(rules);

    print_rules(rules);

    return 0;
}


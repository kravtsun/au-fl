#include <iostream>
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

    return 0;
}


#include <iostream>
#include <algorithm>
#include "token.h"
#include "grammar_parser.h"

#define all(x) (x).begin(), (x).end()

using namespace std;

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

    int cnt = 0;
    for (auto const &r : rules) {
        cout << "Rule #" << cnt++ << ": " << r.left() << " = ";
        auto const &alternatives = r.alternatives();
        std::copy(all(alternatives), std::ostream_iterator<Alternative>(std::cout, " | "));
        cout << endl;
    }

    return 0;
}


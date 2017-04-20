#include <iostream>
#include <algorithm>
#include "token.h"
#include "grammar_parser.h"
#include "grammar_converter.h"

using namespace std;

void print_rules(const Rules &rules) {
    for (auto const &r : rules) {
        cout << r.left() << " = " << r.right() << endl;
    }
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    GrammarParser gp(cin);

    std::string start_name;
    std::getline(cin, start_name);

    Rules all_rules;
    while (gp) {
        Rules rules = gp.parse_line();
        std::copy(all(rules), back_inserter(all_rules));
    }

    GrammarConverter gc{start_name, all_rules};
    gc.convertToChomsky();

    print_rules(gc.rules());

    return 0;
}


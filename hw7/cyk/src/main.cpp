#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstring>

#include "grammar.h"
#include "grammar_checker.h"

using namespace std;

bool has_argument(int argc, char **argv, const char *args) {
    return std::any_of(argv, argv + argc, [args](const char *s) -> bool { return strcmp(s, args) == 0; });
}

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << "USAGE: " << argv[0] << " grammar_filepath input_sequence_filepath [--dot]" << std::endl;
        return 1;
    }

    std::ifstream grammar_fin(argv[1]);

    Grammar gr(grammar_fin);
    gr.normalize();
    GrammarChecker gc(gr);
    if (has_argument(argc, argv, "--verbose")) {
        gr.print_rules();
    }

    std::ifstream fin(argv[2]);
    if (!fin) {
        std::cerr << "Failed to open: " << argv[2] << std::endl;
        return 1;
    }

    std::string word;
    std::getline(fin, word);
    bool result = gc.check(word);

    if (result) {
        if (has_argument(argc, argv, "--dot")) {
            std::cout << gc.build_tree() << std::endl;
        }
        else {
            gc.show_table(has_argument(argc, argv, "--csv"));
        }
    }

    return !result;
}

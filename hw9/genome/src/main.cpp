#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstring>

#include "grammar.h"
#include "grammar_checker.h"
#include "graph.h"
#include "grammar_generator.h"

bool has_argument(int argc, char **argv, const char *args) {
    return std::any_of(argv, argv + argc, [args](const char *s) -> bool { return strcmp(s, args) == 0; });
}

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << "USAGE: " << argv[0] << " grammar_filepath graph_filepath [--verbose]" << std::endl;
        return 1;
    }

    std::ifstream grammar_fin(argv[1]);

    Grammar gr(grammar_fin);
    gr.normalize();
    assert(gr.isNormal());
    GrammarChecker gc(gr);
    if (has_argument(argc, argv, "--verbose")) {
        gr.print_rules();
    }

    std::ifstream graph_fin(argv[2]);
    Graph g; graph_fin >> g;

    GrammarGenerator gg(gc, g);

    auto v = gg.floyd();

    std::copy(all(v), std::ostream_iterator<GrammarGenerator::generated>(std::cout));

    return 0;
}

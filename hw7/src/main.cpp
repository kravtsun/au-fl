#include <fstream>
#include <iostream>
#include <cstring>

#include "grammar.h"

using namespace std;

int main(int argc, char **argv)
{
    if (argc < 3) {
        std::cerr << "USAGE: " << argv[0] << " grammar_filepath input_sequence_filepath [--dot]" << std::endl;
        return 1;
    }

    std::ifstream grammar_fin(argv[1]);

    Grammar gr(grammar_fin);
    gr.normalize();
#if !PRINT_CSV
    gr.print_rules();
#endif

    std::ifstream fin(argv[2]);
    if (!fin) {
        std::cerr << "Failed to open: " << argv[2] << std::endl;
        return 1;
    }

    std::string word;
    std::getline(fin, word);
    bool result = gr.check(word);

    if (result) {
        bool print_dot = false;
        for (int i = 3; i < argc; ++i) {
            print_dot = print_dot || strcmp(argv[i], "--dot") == 0;
        }

        if (print_dot) {
            std::cout << gr.tree() << std::endl;
        }
        else {
            gr.show_table();
        }
    }

    return !result;
}

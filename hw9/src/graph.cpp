#include "graph.h"

std::istream &operator>>(std::istream &is, Graph &gr) {
    gr.edges_.clear();
    int n, m;
    is >> n >> m;
    gr.edges_.resize(n);
    forn(ei, m) {
        Graph::vertex_t a, b;
        char c;
        is >> a >> b >> c;
        a--, b--;
        gr.edges_[a].emplace_back(b, c);
    }
    return is;
}

const Graph::vev &Graph::edges() const {
    return edges_;
}

int Graph::size() const {
    return (int)edges_.size();
}

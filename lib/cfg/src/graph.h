#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <iostream>
#include "common.h"

struct Graph {
    using vertex_t = int;
    using edge_t = std::pair<vertex_t, char>;
    using edge_vector = std::vector<edge_t>;
    using vev = std::vector<edge_vector>;
public:
    friend std::istream &operator>>(std::istream &is, Graph &gr) {
        gr.edges_.clear();
        int n, m;
        is >> n >> m;
        gr.edges_.resize(n);
        forn(ei, m) {
            vertex_t a, b;
            char c;
            is >> a >> b >> c;
            a--, b--;
            gr.edges_[a].emplace_back(b, c);
        }
        return is;
    }

    const vev &edges() const {
        return edges_;
    }

    int size() const {
        return (int)edges_.size();
    }
private:
    vev edges_;
};

#endif // GRAPH_H

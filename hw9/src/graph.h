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
    friend std::istream &operator>>(std::istream &is, Graph &gr);

    const vev &edges() const;

    int size() const;
private:
    vev edges_;
};

#endif // GRAPH_H

#!/usr/bin/python3

import argparse
from random import randint

# from random import choices
def choices(seq, k=1):
    from random import choice
    return [choice(seq) for i in range(k)]

def random_list(n, lim):
# return [randint(1, lim) for i in range(n)]
    return choices(range(1, lim+1), k=n)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Generate graph for tRNA search algorithm.")
    parser.add_argument("-n", dest="n", type=int, required=False, help="number of vertexes.", default=100)
    parser.add_argument("-m", dest="m", type=int, required=False, help="number of edges.", default=1000);
    args = parser.parse_args()

    n, m = args.n, args.m
    characters = ['a', 'c', 'g', 't']
    M = 2*m # some more randomization to avoid the same edges.

    initial_edges = zip(random_list(M, n), random_list(M, n), choices(characters, k=M))
    edges = list(set(initial_edges))[:m]
    assert len(edges) == m

    print(n, m)
    for a, b, c in edges:
        print(a, b, c)
    

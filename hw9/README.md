## REQUIREMENTS
* Ubuntu 16.04
* GCC
* make

## USAGE
Run
```bash
make && ./genome <file-with-grammar> <file-with-graph>
```

## Graph file syntax
First line: two numbers `n` and `m`:

* `n` - number of vertexes
* 
* `m` - number of edges

Then on the following `m` lines - edges' description in format: `from to c`, where

* `from` - vertex from which the current edge is going (number from `1` to `n`)

* `to` - vertex to which the current is going (number from `1` to `n`)

* `c` - edge symbol (one of `{'a', 'c', 'g', 't'}`)

Examples are stored with extension `*.g` in folder `test`

## Grammar file syntax
Following the same rules as in the previous homework. Examples are stored in folder `test`

## t-RNA search
In order to do search tRNA in metagenome set <some-graph> you can run
```bash
make && ./genome test/trna.gr <some-graph>
```


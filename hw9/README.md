# Context Free Grammar Parser and Checker

## Requirements
* Ubuntu 16.04
* GCC
* qmake (tested on version installed with `sudo apt-get install qt5-default`

## USAGE
run `qmake && make -f Makefile.hw9` and `genome/genome <file-with-grammar> <file-with-graph> [--dot|--csv]`. 
### Options:
* `--dot` - get in stdout CSV version of non-terminals which match on input sequence (and some additional info).
* `--csv` - review grammar rules at Chomsky normal form.

## Graph file syntax
first line: two numbers `n` and `m`:
* `n` - number of vertexes
* `m` - number of edges
Then on the following `m` lines - edges' description in format: `from to c`, where
* `from` - vertex from which the current edge is going (number from `1` to `n`)
* `to` - vertex to which the current is going (number from `1` to `n`)
* `c` - edge symbol (one of `{'a', 'c', 'g', 't'}`

## Grammar file syntax
Following the same rules as in the previous homework.

## trna search
In order to do search tRNA in metagenome set <some-graph> you can run
``` qmake && make -f Makefile.hw9 && genome/genome genome/trna_hom.gr <some-graph>```




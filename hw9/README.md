## REQUIREMENTS
* Ubuntu 16.04
* GCC
* qmake (tested on version installed with `sudo apt-get install qt5-default`)

## USAGE
Run 
```qmake && make && genome/genome <file-with-grammar> <file-with-graph>```

## Graph file syntax
First line: two numbers `n` and `m`:
* `n` - number of vertexes
* `m` - number of edges
Then on the following `m` lines - edges' description in format: `from to c`, where
* `from` - vertex from which the current edge is going (number from `1` to `n`)
* `to` - vertex to which the current is going (number from `1` to `n`)
* `c` - edge symbol (one of `{'a', 'c', 'g', 't'}`)
Example is stored with extension `*.g` in folder `genome/test`

## Grammar file syntax
Following the same rules as in the previous homework. Examples are stored in folder `genome/test`

## t-RNA search
In order to do search tRNA in metagenome set <some-graph> you can run
``` qmake && make && genome/genome genome/test/trna_hom.gr <some-graph>```


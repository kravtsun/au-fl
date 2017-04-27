# Context Free Grammar Parser and Checker

## Requirements
* Ubuntu 16.04
* GCC
* qmake (tested on version installed with `sudo apt-get install qt5-default`

## USAGE
run `make` and `cyk/cyk <file-with-grammar> <file-with-input-sequence> [--dot|--csv]`. 
### Options:
* `--dot` - get in stdout CSV version of non-terminals which match on input sequence (and some additional info).
* `--csv` - review grammar rules at Chomsky normal form.


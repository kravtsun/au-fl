# Context Free Grammar Parser and Checker

## USAGE
run `make` and `./a.out <file-with-grammar> <file-with-input-sequence> [--dot]`. If option `--dot` is specified AFTER file paths, 
my console application puts into stdout CSV-style version of non-terminals which match on input sequence (and some additional info).

### Review grammar rules at Chomsky normal form.
At the second line of `Makefile` change its ending from `-DPRINT_CSV=1` to `-DPRINT_CSV=0` and follow the previous howto.


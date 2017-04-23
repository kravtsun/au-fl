# L Language Syntax Analyzer
Flex + GNU Bison based.
Target platform is Ubuntu 16.04.

## PREREQUISITES:
* Ubuntu 16.04
* GCC with C++11 support (via `-std=c++11`).
* flex - the fast lexical analyser generator. It can be easily installed via command:
* bison - GNU Project parser generator (yacc replacement)
* dot - filter for drawing directed graphs (part of GraphViz framework)

Apart from OS and compiler all prerequisites can be installed via command:
``` sh
sudo apt-get install flex bison graphviz
```

## USAGE:
For running all tests (contained in `tests` folder) run `make test`. 
In order to use the syntax analyzer on your single source file `{YOUR_FILE}` (and create image `{YOUR_FILE}.png` with abstract syntax tree), use command 
```sh
INPUT={YOUR_FILE} make run
```
**NOTE:** it also creates tree description in dot format in `{YOUR_FILE}.dot` file.

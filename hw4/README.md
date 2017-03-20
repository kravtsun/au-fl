# L Language Lexer 
Flex-based. 

Target platform is Ubuntu 16.04.

## PREREQUISITES:
* Ubuntu 16.04
* flex - the fast lexical analyser generator. It can be easily installed via command:
`sudo apt-get install flex`.
* GCC with C++11 support (via `-std=c++11`).

## USAGE:
For running all tests (contained in `tests` folder) run `make test`. In order to use the lexer on your single 
source file `FILE`, use command `make && ./llexer < FILE`.

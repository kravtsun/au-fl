FLEX=flex -l
SRC_DIR=src
TEST_DIR=test
SPEC_FILE=$(SRC_DIR)/main.y
HEADERS=$(SRC_DIR)/tokenizer.h $(SRC_DIR)/token.h
SOURCES=$(SRC_DIR)/token.cpp
LEXER_SRC=$(SRC_DIR)/lex.yy.cc
EXEC=./llexer
INPUT_EXT=.l

default: $(EXEC)

$(EXEC): $(LEXER_SRC) $(SOURCES) $(HEADERS)
	g++ -std=c++11 -Isrc $(SOURCES) $(LEXER_SRC) -o $(EXEC) -lfl

$(LEXER_SRC): $(SPEC_FILE)
	$(FLEX) -o $(LEXER_SRC) $(SPEC_FILE)

test: $(EXEC)
	for f in $(TEST_DIR)/*.l; do \
		name="$${f%$(INPUT_EXT)}";\
		check="$$name".a;\
		$(EXEC) < $${f} | diff - "$$check" > /dev/null || echo "TEST $$f failed!" && $(EXEC) < $${f} > $${name}.out;\
	done;

clean:
	rm -f $(EXEC) $(LEXER_SRC) test/*.out

.PHONY: test clean

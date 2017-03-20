FLEX=flex
SRC_DIR=src
TEST_DIR=test
SPEC_FILE=$(SRC_DIR)/main.y
LEXER_SRC=$(SRC_DIR)/lex.yy.cc
EXEC=bin/a.out
INPUT_EXT=.l

default: $(EXEC)

$(EXEC): $(LEXER_SRC)
	g++ -std=c++11 $(LEXER_SRC) -o $(EXEC) -lfl

$(LEXER_SRC): $(SPEC_FILE)
	$(FLEX) -o $(LEXER_SRC) $(SPEC_FILE)

test: $(EXEC)
	for f in $(TEST_DIR)/*.l; do \
		name="$${f%$(INPUT_EXT)}";\
		check="$$name".a;\
		$(EXEC) < $${f} | diff - "$$check" > /dev/null || echo "TEST $$f failed!" && $(EXEC) < $${f} > $${name}.out;\
	done;

clean:
	rm -f $(EXEC) $(LEXER_SRC)

.PHONY: test clean

FLEX=flex -+
SPEC_FILE=main.y
LEXER_SRC=lex.yy.cc
EXEC=./a.out
INPUT_EXT=.l

default: $(EXEC)

$(EXEC): $(LEXER_SRC)
	g++ -std=c++11 $(LEXER_SRC) -o $(EXEC) -lfl

$(LEXER_SRC): $(SPEC_FILE)
	$(FLEX) -o $(LEXER_SRC) $(SPEC_FILE)

test: $(EXEC)
	for f in test/*.l; do \
		name="$${f%$(INPUT_EXT)}";\
		check="$$name".a;\
		$(EXEC) < $${f} | diff - "$$check" > /dev/null || echo "TEST $$f failed!";\
	done;

clean:
	rm -f $(EXEC) $(LEXER_SRC)

.PHONY: test clean

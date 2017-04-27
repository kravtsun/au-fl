CXX = g++
CXXFLAGS = -O2 -Wall -std=c++14
LDFLAGS = 

EXEC = cyk/cyk

CYK_SOURCES = $(wildcard cyk/src/*)
CFG_SOURCES = $(wildcard ../lib/cfg/src/*)

QMAKE_MAKEFILE = Makefile.hw7

EXEC: %(QMAKE_MAKEFILE) $(CYK_SOURCES) $(CFG_SOURCES)
	qmake && make -j $$(nproc) -f $(QMAKE_MAKEFILE); \cd -

%(QMAKE_MAKEFILE):
	qmake

GRAMMAR_EXT=".gr"
define test_grammar = 
	for t in $(1)/$(2)/*.txt; \
	do                        \
		output="$$t".output;  \
		rm -f "$$output";     \
		./$(EXEC) $(1)$(GRAMMAR_EXT) $$t 1>$$output; \
		if [ $$? -eq $(3) ]; \
		then \
			>&2 echo test "$$t" failed!; \
		fi; \
	done
endef

test: $(EXEC)
	for gr in cyk/test/*.gr; do \
		grname="$${gr%$(GRAMMAR_EXT)}"; \
		$(call test_grammar,"$$grname","wrong",0); \
		$(call test_grammar,"$$grname","right",1); \
	done;

clean: 
	qmake && make clean -f $(QMAKE_MAKEFILE) && rm -f $(QMAKE_MAKEFILE)

.PHONY: clean all test


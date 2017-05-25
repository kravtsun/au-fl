CXX = g++
CXXFLAGS = -O2 -Wall -Werror -std=c++14
LDFLAGS = 

EXE = genome
SRCDIR = src
BINDIR = bin

OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(wildcard $(SRCDIR)/*.cpp))

all: $(EXE)

$(EXE): $(BINDIR) $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXE) $(LDFLAGS)
	
$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -MMD -o $@ $<

include $(wildcard $(BINDIR)/*.d)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(BINDIR) $(EXE) test/*.out && find . -name \*.output -delete

GRAMMAR_EXT=".gr"
define test_grammar = 
	for t in $(1)/$(2)/*.txt; \
	do                        \
		output="$$t".output;  \
		rm -f "$$output";     \
		./$(EXE) $(1)$(GRAMMAR_EXT) $$t --grammar 1>$$output; \
		if [ $$? -eq $(3) ]; \
		then \
			>&2 echo grammar test "$$t" failed!; \
		fi; \
	done
endef

testgr: $(EXE)
	for gr in test/cyk/*.gr; do \
		grname="$${gr%$(GRAMMAR_EXT)}"; \
		$(call test_grammar,"$$grname","wrong",0); \
		$(call test_grammar,"$$grname","right",1); \
	done;

test: $(EXE) testgr
	if cd test; then \
		for grammar in *.gr; \
			do grammar="$${grammar%.gr}"; \
			for graph in *.g; \
				do graph="$${graph%.g}"; \
					test="$${grammar}"_"$${graph}"; \
					output="$$test".out; \
					check="$$test".a; \
					../genome "$$grammar".gr "$$graph".g > "$$output" && diff "$$output" "$$check" > /dev/null || >&2 echo "graph test $$test failed"; \
				done; \
		done; \
		cd -; \
	else \
		>&2 echo "failed: test directory is not found."; \
	fi

.PHONY: clean all test testgr

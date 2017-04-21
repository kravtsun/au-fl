CXX = g++
CXXFLAGS = -O2 -Wall -std=c++14 -DPRINT_CSV=1
LDFLAGS = 

EXEC = a.out
SRCDIR = src
BINDIR = bin

OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(wildcard $(SRCDIR)/*.cpp))

all: $(EXEC)

$(EXEC): $(BINDIR) $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXEC) $(LDFLAGS)
	
$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -MMD -o $@ $<

include $(wildcard $(BINDIR)/*.d)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(BINDIR) $(EXEC)
	find . -name \*.output -or -name \*.dot -delete

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
	for gr in test/*.gr; do \
		grname="$${gr%$(GRAMMAR_EXT)}"; \
		$(call test_grammar,"$$grname","wrong",0); \
		$(call test_grammar,"$$grname","right",1); \
	done;

.PHONY: clean all test


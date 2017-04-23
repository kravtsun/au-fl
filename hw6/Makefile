DEBUG_FLAGS=-DYYDEBUG=1 
CFLAGS=-std=c++11 #-O0 -ggdb $(DEBUG_FLAGS)
OUTPUT_DIR=bin
SRC_DIR=src
SOURCES=$(SRC_DIR)/ast.h
LEX_IN=$(SRC_DIR)/main.y
LEX_OUT=$(SRC_DIR)/lex.yy.cpp
BISON_IN=$(SRC_DIR)/LGrammar.g
BISON_OUT=$(SRC_DIR)/LGrammar.tab.g
EXEC=$(OUTPUT_DIR)/a.out
FLEX=flex #--debug
BISON=bison --defines #--graph
DOT_FILE=$(OUTPUT_DIR)/LGrammar.dot

default: $(EXEC)

run: $(EXEC)
	./$(EXEC) ${INPUT} && dot -Tpng ${INPUT}.dot > ${INPUT}.png && eog ${INPUT}.png

$(EXEC): $(LEX_OUT) $(BISON_OUT) $(SOURCES)
	g++ $(CFLAGS) $(LEX_OUT) -o $(EXEC)

$(LEX_OUT): $(BISON_OUT) $(LEX_IN)
	$(FLEX) -o $(LEX_OUT) $(LEX_IN)

$(BISON_OUT): $(BISON_IN)
	$(BISON) -o $(BISON_OUT) $(BISON_IN)
	# dot -Tpng $(subst .tab.g,.dot,$(BISON_OUT)) > $(BISON_IN).png


.PHONY: clean test

clean: 
	mkdir -p $(OUTPUT_DIR)
	rm -f test/*.L.dot
	rm -f $(OUTPUT_DIR)/*{.out,.dot,.output,.png} $(LEX_OUT) $(BISON_OUT)


test: $(EXEC)
	for f in test/*.L; do \
		rm -f "$$f".dot; \
		./$(EXEC) "$$f";\
		diff "$$f".dot "$${f%.L}.dot" >/dev/null; \
		if [ $$? -ne 0 ]; then \
			>&2 echo test "$$f" failed!; \
		fi;\
	done

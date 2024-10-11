# SETTINGS
COMPILER = gcc
FLAGS = -Wall -Werror -std=c11 -g
COMP = $(COMPILER) $(FLAGS) $^ -o $@

# COMMANDS
.PHONY: all
all: firstfit bestfit

.PHONY: clean
clean:
	rm -f firstfit
	rm -f bestfit

# EXECUTABLE FILES
firstfit: src/main.c src/node_search_firstfit_impl.c
	$(COMP)

bestfit: src/main.c src/node_search_bestfit_impl.c
	$(COMP)

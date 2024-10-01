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
firstfit: firstfit.c
	$(COMP)

bestfit: bestfit.c
	$(COMP)

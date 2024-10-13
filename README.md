# Assignment 2 - Operating Systems Sem 2 2024

https://rmit.instructure.com/courses/125046/assignments/985713

Authors
-----------------------------
**Joshua James Catherine Jane Moylan**
- Student ID: s3906557 
- Email: s3906557@student.rmit.edu.au

**Ridge Tagala**
- Student ID: s3934367
- Email: s3934367@student.rmit.edu.au

Introduction
-----------------------------

This program is a demonstration of managing allocated memory in a process.

It reads a `datafile` (see *Running the Program*), containing instructions to allocate and deallocate memory.

The program requests additional space from the operating system where necessary, reuses space where possible, and gives space back when the program has ended.

It keeps track of its state using a `freed_list` and `allocated_list` (see `main.c`), which are linked lists of data structures specifying where memory has been allocated and how much has been allocated there (see `alloc_node.h`).

Compiling the Program
-----------------------------
The following `make` commands are available:
- `make all` performs `make bestfit` and `make firstfit`.
- `make bestfit` creates the `bestfit` executable.
- `make firstfit` creates the `firstfit` executable.
- `make clean` removes the `bestfit` and `firstfit` executables.

Additionally, `p2_gen.sh` produces the contents of a valid `datafile` in stdout (see *Running the Program*).

Running the Program
-----------------------------

Either the `bestfit` or `firstfit` executables must be executed with a single argument containing the filepath of a `datafile`.

For instance, compiling and running the program may look like:
```
make all
p2_gen.sh 20 > datafile
./bestfit datafile
```

The program will output three sections to `stdout`:
- The **`Action Log`** is a log of all instructions executed from the `datafile`.
    - **`Freeing`** refers to space being added to the `freed_list` from the `allocated_list`. It occurs when a `dealloc` instruction is given.
    - **`Requesting`** refers to making space for data by requesting more heap space from the operating system. It occurs when an `alloc` instruction is given and there is no suitable chunk in the `freed_list`.
    - **`Reusing`** refers to making space for data by reusing space in the `freed_list`. It occurs when an `alloc` instruction is given and there is a suitable chunk in the `freed_list`.
- The **`Allocated List`** is the final state of the `allocated_list` linked list after all instructions in the `datafile` have been parsed.
- The **`Freed List`** is the final state of the `freed_list` likned list after all instructions in the `datafile` have been parsed.

Program Structure
-----------------------------

Both the `firstfit` and `bestfit` executables contain `main.c`, but each executable contains their own `node_search.h` implementation: `node_search_firstfit_impl.c` and `node_search_bestfit_impl.c`, respectively.
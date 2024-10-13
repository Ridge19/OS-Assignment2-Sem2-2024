#ifndef OSP_ASSIGNMENT_2_MAIN_H
#define OSP_ASSIGNMENT_2_MAIN_H

#include "alloc_node.h"

int brk(void *end_data_segment);
void * sbrk(ptrdiff_t chunk_size);

void * alloc(size_t chunk_size);
void dealloc(void *memory_chunk);

void print_allocated_list(AllocNode *alloc_linked_list);
void print_freed_list(AllocNode *alloc_linked_list);

int main(int argc, char **argv);

#endif
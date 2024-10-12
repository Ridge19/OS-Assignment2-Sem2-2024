#ifndef OSP_ASSIGNMENT_2_MAIN_H
#define OSP_ASSIGNMENT_2_MAIN_H

#include "alloc_node.h"

int brk(void *end_data_segment);
void * sbrk(ptrdiff_t chunk_size);

AllocNode * create_node(size_t chunk_size);
void * alloc(size_t chunk_size);
void dealloc(void *memory_chunk);
void print_list(AllocNode *head, const char *list_name);
int main(int argc, char **argv);

#endif
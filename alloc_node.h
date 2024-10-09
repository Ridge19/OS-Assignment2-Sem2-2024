#ifndef OSP_ASSIGNMENT_2_ALLOC_NODE_H
#define OSP_ASSIGNMENT_2_ALLOC_NODE_H

#include <stdlib.h>

// Node structure for linked list
typedef struct AllocNode {
    void *memory_block;   // Pointer to the allocated memory
    size_t size;          // Size of the allocated memory
    struct AllocNode *next;
} AllocNode;

#endif
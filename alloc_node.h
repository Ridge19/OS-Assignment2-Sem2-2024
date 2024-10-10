#ifndef OSP_ASSIGNMENT_2_ALLOC_NODE_H
#define OSP_ASSIGNMENT_2_ALLOC_NODE_H

#include <stdlib.h>

// Node structure for linked list
typedef struct AllocNode {
    void *memory_chunk;   // Pointer to the allocated memory
    size_t chunk_size;    // Size of the allocated memory
    struct AllocNode *next;
} AllocNode;

#endif
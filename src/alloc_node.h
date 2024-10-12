#ifndef OSP_ASSIGNMENT_2_ALLOC_NODE_H
#define OSP_ASSIGNMENT_2_ALLOC_NODE_H

#include <stddef.h> // size_t

// In its current state, sizeof(AllocNode) is 24.
// This assignment requires all memory to be allocated in specific partition sizes.
#define ALLOC_NODE_SIZE 32

// Node structure for linked list
typedef struct AllocNode {
    void *memory_chunk;   // Pointer to the allocated memory
    size_t chunk_size;    // Size of the allocated memory
    struct AllocNode *next;
} AllocNode;

#endif
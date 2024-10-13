#ifndef OSP_ASSIGNMENT_2_ALLOC_NODE_H
#define OSP_ASSIGNMENT_2_ALLOC_NODE_H

#include <stddef.h> // for size_t

/*
 * This assignment requires all memory to be allocated in specific partition sizes. Hence, an amount
 * from the permitted list that is capable of storing AllocNodes has been chosen.
 */
#define ALLOC_NODE_SIZE 32

/*
 * AllocNode is a struct that stores details about allocated memory, and also points to another
 * AllocNode. This allows for the creation of linked lists of AllocNodes.
 */
typedef struct AllocNode {
    void *memory_chunk;         // Pointer to the allocated memory
    size_t chunk_size;          // Size of the allocated memory
    size_t used_size;           // Size of the amount of memory that has actually been used
    struct AllocNode *next;     // Pointer to the next AllocNode
} AllocNode;

#endif
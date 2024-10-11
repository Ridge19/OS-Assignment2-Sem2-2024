#include <malloc.h> // for ptrdiff_t
#include <stdint.h> // for int8_t
#include <stdio.h>
#include <stdlib.h>

#include "alloc_node.h"
#include "node_search.h"

int brk(void *end_data_segment);
void *sbrk(ptrdiff_t increment);

// Linked lists to track allocated and freed memory
static AllocNode *allocated_list = NULL;
static AllocNode *freed_list = NULL;

// Function to create a new AllocNode
AllocNode * create_node(size_t chunk_size) {

    // Create the AllocNode by growing the address space
    AllocNode *allocation = sbrk((ptrdiff_t) 0);
    if (brk(allocation + sizeof(AllocNode)) < 0) {
        puts("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Create the AllocNode's memory_chunk by growing the address space
    void* memory_chunk = sbrk((ptrdiff_t) 0);
    if (brk(memory_chunk + chunk_size) < 0) {
        puts("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Set the values of the AllocNode
    allocation->memory_chunk = memory_chunk;
    allocation->chunk_size = chunk_size;
    allocation->next = NULL;

    // Return the AllocNode
    return allocation;
}

// Core allocation function
void * alloc(size_t chunk_size) {

    // Attempt to get a node from the Freed List
    AllocNode * allocation = find_and_pop_suitable_node(&freed_list, chunk_size);

    // If a node was obtained from the Freed List, zero its memory_chunk
    if (allocation != NULL) {
        for (unsigned long i = 0; i < allocation->chunk_size; ++i) {
            int8_t* current_byte = allocation->memory_chunk + i;
            *current_byte = 0;
        }
    }

    // If a node was not be obtained from the Freed List, make a new one
    else {
        allocation = create_node(chunk_size);
    }

    // Add the node to the front of the Allocated List
    allocation->next = allocated_list;
    allocated_list = allocation;

    // Write allocation operation to the console
    printf("Allocating %zu bytes at address %p\n", chunk_size, allocation->memory_chunk);

    return allocation->memory_chunk;
}

// Core deallocation function
void dealloc(void *memory_chunk) {

    // Search for the block in the allocated list
    AllocNode *previous = NULL;
    AllocNode *current = allocated_list;
    while (current != NULL) {
        if (current->memory_chunk == memory_chunk) {

            // Remove the node from the Allocated List
            if (previous == NULL) {
                allocated_list = current->next;
            } else {
                previous->next = current->next;
            }
            
            // Add the node to the Freed List
            current->next = freed_list;
            freed_list = current;
            
            // Write deallocation operation to the console
            printf("Freeing %zu bytes at address %p\n", current->chunk_size, current->memory_chunk);

            return;
        }

        previous = current;
        current = current->next;
    }

    printf("Block not found in allocated list!\n");
}

// Function to print the list (for debugging)
void print_list(AllocNode *head, const char *list_name) {
    printf("%s:\n", list_name);
    while (head != NULL) {
        printf("Memory Block: %p, Size: %zu bytes -> ", head->memory_chunk, head->chunk_size);
        head = head->next;
    }
    printf("NULL\n");
}

// Main function to demonstrate the process
int main(int argc, char *argv[]) {

    // Allocate memory using alloc function
    void *block1 = alloc(100); // Allocate 100 bytes
    void *block2 = alloc(200); // Allocate 200 bytes
    void *block3 = alloc(300); // Allocate 300 bytes
    
    // Print the allocated list for debugging
    print_list(allocated_list, "Allocated List");

    // Deallocate memory using dealloc function
    dealloc(block2); // Free block2
    dealloc(block1); // Free block1
    
    // Print both lists after freeing some blocks for debugging
    print_list(allocated_list, "Allocated List");
    print_list(freed_list, "Freed List");

    // Cleanup remaining allocated memory
    dealloc(block3);

    return 0;
}

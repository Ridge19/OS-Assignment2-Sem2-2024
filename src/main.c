#include <malloc.h> // for ptrdiff_t
#include <stdint.h> // for int8_t
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for strcmp()

#include "main.h"

#include "alloc_node.h"
#include "node_search.h"

// Linked lists to track allocated and freed memory
static AllocNode *allocated_list = NULL;
static AllocNode *freed_list = NULL;





/*
 * This function accepts a size_t representing the amount of memory requested in bytes, and returns
 * a void* corresponding to a place in memory which can safely store that much data *at minimum*.
 * 
 * The space is requested from the operating system if necessary, or recycled from the freed_list if
 * possible.
 * 
 * This operation is recorded by adding a node to the allocated_list. This node was either
 * previously on the freed_list if recycled, or generated anew if the address space was requested
 * from the operating system.
 */
void * alloc(size_t chunk_size) {

    // Attempt to get a node from the Freed List
    AllocNode * allocation = find_and_pop_suitable_node(&freed_list, chunk_size);

    // If an AllocNode was obtained from the Freed List, modify it
    if (allocation != NULL) {

        // Zero the AllocNode's memory_chunk
        for (size_t i = 0; i < allocation->chunk_size; ++i) {
            int8_t *current_byte = allocation->memory_chunk + i;
            *current_byte = 0;
        }

        // Replace the AllocNode's used_size
        allocation->used_size = chunk_size;

        // Write allocation operation to the console
        printf("Reusing    %3liB at %p\n", allocation->chunk_size, allocation->memory_chunk);
    }

    // If an AllocNode was not obtained from the Freed List, make a new one
    else {

        // Store the originally requested size as a new variable
        size_t used_size = chunk_size;

        // Adjust the value of chunk_size to an appropriate power of two
        if (chunk_size > 512) {
            puts("ERROR: Illegal argument, chunk_size must be 512 or less.\n");
            exit(EXIT_FAILURE);
        }
        else if (chunk_size > 256) chunk_size = 512;
        else if (chunk_size > 128) chunk_size = 256;
        else if (chunk_size > 64) chunk_size = 128;
        else if (chunk_size > 32) chunk_size = 64;
        else if (chunk_size > 0) chunk_size = 32;
        else {
            puts("ERROR: Illegal argument, chunk_size cannot be 0.\n");
            exit(EXIT_FAILURE);
        }

        // Create the AllocNode by growing the address space
        allocation = sbrk((ptrdiff_t) 0);
        if (brk(allocation + ALLOC_NODE_SIZE) < 0) {
            puts("ERROR: Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        // Create the AllocNode's memory_chunk by growing the address space
        void *memory_chunk = sbrk((ptrdiff_t) 0);
        if (brk(memory_chunk + chunk_size) < 0) {
            puts("ERROR: Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        // Set the values of the AllocNode
        allocation->memory_chunk = memory_chunk;
        allocation->chunk_size = chunk_size;
        allocation->used_size = used_size;

        // Write allocation operation to the console
        printf("Requesting %3liB at %p\n", chunk_size, allocation->memory_chunk);
    }

    // Add the node to the front of the Allocated List
    allocation->next = allocated_list;
    allocated_list = allocation;

    return allocation->memory_chunk;
}





/*
 * This function accepts a void* previously returned by alloc(). The void* provided is "given back"
 * (stored in the freed_list), allowing it to be returned by future alloc calls. 
 */
void dealloc(void *memory_chunk) {

    // Iterate through the Allocated List, looking for the AllocNode containing the memory_chunk
    AllocNode *previous = NULL;
    AllocNode *current = allocated_list;
    while (current != NULL) {

        // Detect the AllocNode containing the memory_chunk
        if (current->memory_chunk == memory_chunk) {

            // Remove the AllocNode from the Allocated List
            if (previous == NULL) {
                allocated_list = current->next;
            } else {
                previous->next = current->next;
            }

            // Set the AllocNode's used_size to zero
            current->used_size = 0;
            
            // Add the AllocNode to the Freed List
            current->next = freed_list;
            freed_list = current;
            
            // Write deallocation operation to the console
            printf("Freeing    %3liB at %p\n", current->chunk_size, current->memory_chunk);

            // Exit the function
            return;
        }

        // Move to the next AllocNode in the Allocated List
        previous = current;
        current = current->next;
    }

    // Report failure to find memory_chunk
    puts("ERROR: Cannot deallocate an address that was never allocated.");
    exit(EXIT_FAILURE);
}





/*
 * This is a function designed to neatly print the contents of the allocated_list.
 */
void print_allocated_list(AllocNode *alloc_linked_list) {
    AllocNode *node = alloc_linked_list;
    while (node != NULL) {
        printf("%p %3liB / %3liB -> \n", node->memory_chunk, node->used_size, node->chunk_size);
        node = node->next;
    }
    puts("NULL");
}





/*
 * This is a function designed to neatly print the contents of the freed_list.
 */
void print_freed_list(AllocNode *alloc_linked_list) {
    AllocNode *node = alloc_linked_list;
    while (node != NULL) {
        printf("%p %3liB -> \n", node->memory_chunk, node->chunk_size);
        node = node->next;
    }
    puts("NULL");
}





/*
 * The main function invokes the alloc and dealloc functions according to instructions provided in a
 * file specified as the sole argument. 
 */
int main(int argc, char **argv) {

    // Add newline
    puts("");

    // Handle case where there are too few arguments
    if (argc < 2) {
        puts("ERROR: Too few arguments.\n");
        return EXIT_FAILURE;
    }

    // Handle case where there are too many arguments
    else if (argc > 2) {
        puts("ERROR: Too many arguments.\n");
        return EXIT_FAILURE;
    }

    // Open the first argument as a file
    FILE *data_file = fopen(argv[1], "r");

    // Handle case where file will not open
    if (!data_file) {
        printf("ERROR: File %s could not be accessed.\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Print heading
    puts("Action Log:");

    // Declare buffer
    char buffer[100];

    // Read file and act on instructions
    while (fgets(buffer, sizeof(buffer), data_file) != NULL) {

        // Handle "alloc" case
        if (strncmp(buffer, "alloc:", 6) == 0) {

            // Get amount of bytes from instruction
            size_t chunk_size = (size_t) atoi(buffer + 7);

            // Handle bad amount case
            if (chunk_size <= 0) {
                puts("ERROR: Invalid number of bytes to allocate.\n");
                return EXIT_FAILURE;
            }

            // Execute instruction
            alloc(chunk_size);
        }

        // Handle "dealloc" case
        else if (strncmp(buffer, "dealloc", 7) == 0) {

            // Execute instruction
            if (allocated_list != NULL) dealloc(allocated_list->memory_chunk);
        }

        // Handle bad instruction case
        else {
            puts("ERROR: Instruction was not \"alloc\" or \"dealloc\".\n");
            return EXIT_FAILURE;
        }
    }

    // Add newline
    puts("");

    // Print results
    puts("Allocated List:");
    print_allocated_list(allocated_list);

    puts("\nFreed List:");
    print_freed_list(freed_list);
    
    // Add newline and exit the program
    puts("");
    return EXIT_SUCCESS;
}

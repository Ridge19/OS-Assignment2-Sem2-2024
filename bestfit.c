#include <stdio.h>
#include <stdlib.h>

// Node structure for linked list
typedef struct Node {
    void *memory_block;   // Pointer to the allocated memory
    size_t size;          // Size of the allocated memory
    struct Node *next;
} Node;

// Linked lists to track allocated and freed memory
Node *allocated_list = NULL;
Node *freed_list = NULL;

// Output file pointer for the .txt file
FILE *output_file;

// Function to create a new node
Node* create_node(void *block, size_t size) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) {
        printf("Memory allocation for node failed\n");
        exit(1);
    }
    new_node->memory_block = block;
    new_node->size = size;
    new_node->next = NULL;
    return new_node;
}

// Function to add a node to the linked list
void add_node(Node **head, void *block, size_t size) {
    Node *new_node = create_node(block, size);
    new_node->next = *head;
    *head = new_node;
}

// Core allocation function
void* alloc(size_t chunk_size) {
    void *block = malloc(chunk_size);
    if (block == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    add_node(&allocated_list, block, chunk_size);  // Add block with size to the allocated list

    // Write allocation operation to the text file
    fprintf(output_file, "Allocating %zu bytes at address %p\n", chunk_size, block);

    return block;
}

// Core deallocation function
void dealloc(void *chunk) {
    // Search for the block in the allocated list
    Node *prev = NULL, *current = allocated_list;
    
    while (current != NULL) {
        if (current->memory_block == chunk) {
            // Remove the node from the allocated list
            if (prev == NULL) {
                allocated_list = current->next;
            } else {
                prev->next = current->next;
            }
            
            // Add the node to the freed list
            add_node(&freed_list, chunk, current->size);
            
            // Write deallocation operation to the text file
            fprintf(output_file, "Freeing %zu bytes at address %p\n", current->size, chunk);

            // Free the memory
            free(chunk);

            // Free the node itself
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Block not found in allocated list!\n");
}

// Function to print the list (for debugging)
void print_list(Node *head, const char *list_name) {
    printf("%s:\n", list_name);
    while (head != NULL) {
        printf("Memory Block: %p, Size: %zu bytes -> ", head->memory_block, head->size);
        head = head->next;
    }
    printf("NULL\n");
}

// Main function to demonstrate the process
int main() {
    // Open file for writing text output
    output_file = fopen("memory_operations.txt", "w");
    if (output_file == NULL) {
        printf("Failed to open file for writing\n");
        return 1;
    }

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

    // Close the output file
    fclose(output_file);

    return 0;
}

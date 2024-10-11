#include "node_search.h"

AllocNode * find_and_pop_suitable_node(AllocNode **alloc_linked_list, size_t chunk_size) {

    // Initialise previous and current nodes
    AllocNode *previous = NULL;
    AllocNode *current = *alloc_linked_list;

    // Iterate through the linked list
    while (current != NULL) {
        
        // Check if the current node is at least the requested size
        if (current->chunk_size >= chunk_size) {

            // Remove the node from the linked list
            if (previous == NULL) {
                *alloc_linked_list = current;
            }
            else {
                previous->next = current->next;
            }

            // Return the node
            return current;
        }
        
        // Move to the next node
        previous = current;
        current = current->next;
    }

    // Return NULL by default
    return NULL;
}
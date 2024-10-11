#include "node_search.h"

AllocNode * find_and_pop_suitable_node(AllocNode **alloc_linked_list, size_t chunk_size) {
    // initialise variables 
    AllocNode *previous = NULL;
    AllocNode *current = *alloc_linked_list;
    AllocNode *best_fit = NULL;
    AllocNode *best_fit_previous = NULL;

    // iterate through linked list 
    while (current != NULL)
    {
        // check current node size is >= requested size
        if (current->chunk_size >= chunk_size)
        {
            // check if best fit is found
            if (best_fit == NULL || current->chunk_size < best_fit->chunk_size)
            {
                best_fit = current;
                best_fit_previous = previous;
            }
        }

        // next node 
        previous = current;
        current = current->next;

        // if best-fit node was found, remove from linked list 
        if (best_fit != NULL)
        {
            if (best_fit_previous == NULL)
            {
                *alloc_linked_list = best_fit->next;
            }
            else
            {
                best_fit_previous->next = best_fit->next;
            }

            return best_fit;
        }
    }
    
    return NULL;
}
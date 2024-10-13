#ifndef OSP_ASSIGNMENT_2_NODE_SEARCH_H
#define OSP_ASSIGNMENT_2_NODE_SEARCH_H

#include "alloc_node.h"

/*
 * Definition for method implemented by either node_search_firstfit_impl.c or
 * node_search_best_fit_impl.c, depending on the produced executable.
 */
AllocNode * find_and_pop_suitable_node(AllocNode **alloc_linked_list, size_t chunk_size);

#endif
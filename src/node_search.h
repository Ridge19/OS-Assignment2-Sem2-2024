#ifndef OSP_ASSIGNMENT_2_NODE_SEARCH_H
#define OSP_ASSIGNMENT_2_NODE_SEARCH_H

#include "alloc_node.h"

AllocNode * find_and_pop_suitable_node(AllocNode **alloc_linked_list, size_t chunk_size);

#endif
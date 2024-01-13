// $Id: free_list

#ifndef FREE_LIST_H
#define FREE_LIST_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// A ToFree node is a node that can be freed later down the line.
// You are also able to define "mark", which can be checked on or off,
// if the node is meant to be automatically removed.
typedef struct ToFree {
  void*   ptr;
  bool    mark;
  size_t  size;

  struct ToFree * next;
} ToFree;

// A "Free List" is a Linked List-esque structure in which 
// each block of memory allocated to it is added as a node of the tree.
// This is similar if not exact to the concept of "Arena Allocation", where
// there's one initializer, and one free, and there can be a variation of
// allocations.
typedef struct Free_List {
  ToFree *head;
} Free_List;

// Creates a new free list, verifying memory using `assert()`
Free_List* list_new();

// Frees the list's nodes, skipping nodes that are not marked.
void list_free_nodes(Free_List* list);

// frees the entire list. including any extra memory utilized by it.
void list_free(Free_List* list);

// creates a block of memory and appends it to the freelist using
// `list_append(void*);`, setting the nodes marked field to MARK.
//
//  node marking:
//    If a node is marked, then when `list_free_nodes()` is called;
//    then that specific node will not be freed. this systematic
//    approach is good for blocks of memory that are meant to be 
//    deallocated separately; in the worst case event--checks are
//    in place to ensure that stuff like double frees don't happen 
//    with this function, and any others.
//
void* list_create(Free_List* list, size_t bytes);

// reallocates a pointer that was allocated with list_create().
void* list_realloc(Free_List* list, void* old_pointer, size_t new_bytes);

// Creates a ToFree node and returns it.
ToFree * free_node(void* pointer);

#endif /* FREE_LIST_H */


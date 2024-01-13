// $Id: free_list.c

#include "free_list.h"

#include <assert.h>

Free_List *
list_new ()
{
  Free_List *listp = malloc (sizeof (Free_List));
  assert (listp);

  listp->head = NULL;

  assert (!listp->head);

  return listp;
}

void *
list_create (Free_List *list, size_t bytes)
{
  /* allocate size BYTES, unaligned (not technically but yk) */
  void *memory_pointer = malloc (bytes);
  assert (memory_pointer);

  if (!list->head)
    {
      list->head = free_node (memory_pointer);
      assert (list->head);

      return list->head->ptr;
    }
  else
    {
      ToFree *tmp = list->head;

      while (tmp->next)
        {
          tmp = tmp->next;
        }
      assert (tmp);

      tmp->next = free_node (memory_pointer);
      tmp->size = bytes;
      
      tmp = tmp->next;
      return tmp->ptr;
    }

  return NULL;
}

void *
list_realloc (Free_List *list, void *old_pointer, size_t new_bytes)
{
  assert(list);
  assert(old_pointer);

  ToFree *tmp = list->head;

  while (tmp) {
    if (tmp->ptr == old_pointer) {
      void* ptr = list_create(list, new_bytes);

      assert(ptr != NULL);

      memcpy(ptr, old_pointer, tmp->size); // should work. right?

      return ptr;
    }

    tmp = tmp->next;
  }

  return NULL;
}

ToFree *
free_node (void *pointer)
{
  assert (pointer);

  ToFree *node = malloc (sizeof (ToFree));

  assert (node);

  node->ptr = pointer;
  node->mark = true;
  node->next = NULL;

  return node;
}

// free all nodes in LIST
void
list_free_nodes (Free_List *list)
{
  assert (list);

  ToFree *tmp = list->head;
  assert (tmp);
  ToFree *hd = tmp;

  while (tmp)
    {
      hd = tmp;

      tmp = tmp->next;

      if (hd->mark)
        {
          // we don't want to see this again
          hd->mark = false;
          free (hd->ptr);
          free (hd);
        }
    }

  list->head = NULL;
}

void
list_free (Free_List *list)
{
  assert (list);

  if (list->head)
    {
      list_free_nodes (list);
    }

  if (list)
    {
      free (list);
    }
}

int
main (void)
{
  Free_List *list = list_new ();

  for (int i = 0 ; i < 5000; ++ i) {
    list_create(list, i);
  }
    
  list_free_nodes (list);

  list_free (list);
}


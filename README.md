# FreeList

A simple FreeList implementation in C, primarily designed just because I wanted a Freelist about now, and I
for some reason struggled to muster up the courage to make one before this point. But now it's here.

## Example Usage

```c
// in free_list.c ...
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

```

## License (AGPL)

The license for this project is the `GNU Affero Public License`.


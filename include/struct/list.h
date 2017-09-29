#ifndef LIST_H
#define LIST_H 1

#include <stddef.h>
#include <stdbool.h>

/*
 * Deque structure.
 */
typedef struct list list;

/*
 * Creates a list whose elements are elm_size bytes long.
 */
list *list_create(size_t elm_size);
/*
 * Frees memory allocated for a given list.
 * If its elements were pointers to allocated memory, they
 * WILL NOT be freed.
 */
void list_destroy(list *l);

/*
 * Returns the number of elements in the list.
 */
size_t list_get_size(const list *l);
/*
 * Equivalent to
 * 		(list_get_size(l) == 0)
 */
bool list_is_empty(const list *l);

/*
 * Add an element to the front of the list.
 */
void list_push_front(list *l, const void *elm);
/*
 * Add an element to the end of the list.
 */
void list_push_back(list *l, const void *elm);
/*
 * If the list is empty, returns false and out is left untouched.
 * Otherwise, remove the element in the front of the list, write it to out
 * and return true.
 */
bool list_pop_front(list *l, void *out);
/*
 * If the list is empty, returns false and out is left untouched.
 * Otherwise, remove the element in the back of the list, write it to out
 * and return true.
 */
bool list_pop_back(list *l, void *out);

#endif
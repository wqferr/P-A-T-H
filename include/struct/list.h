#ifndef LIST_H
#define LIST_H 1

#include <stddef.h>
#include <stdbool.h>

typedef struct list list;

list *list_create(size_t elem_size);
void list_destroy(list *l);

void list_ensure_capacity(list *l, size_t cap);
size_t list_get_length(const list *l);
bool list_is_empty(const list *l);

void list_push_front(list *l, const void *elm_ref);
void list_push_back(list *l, const void *elm_ref);
bool list_pop_front(list *l, void *out);
bool list_pop_back(list *l, void *out);

void list_foreach(list *l, void (*f)(void *));

#endif
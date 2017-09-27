#ifndef LIST_H
#define LIST_H 1

#include <stddef.h>
#include <stdbool.h>

typedef struct list list;

list *list_create(size_t elm_size);
void list_destroy(list *l);

size_t list_get_size(const list *l);
bool list_is_empty(const list *l);

void list_push_front(list *l, const void *elm);
void list_push_back(list *l, const void *elm);
bool list_pop_front(list *l, void *out);
bool list_pop_back(list *l, void *out);

void list_for_each(list *l, void (*f)(void *));

#endif
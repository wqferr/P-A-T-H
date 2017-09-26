#ifndef HEAP_H
#define HEAP_H 1

#include <stddef.h>
#include <stdbool.h>

/* Max heap structure */
typedef struct heap heap;

heap *heap_create(size_t elm_size);
void heap_destroy(heap *h);

void heap_ensure_capacity(heap *h, size_t cap);
size_t heap_get_size(const heap *h);
bool heap_is_empty(const heap *h);

bool heap_add(heap *h, const void *elm, int pr);
int heap_pop(heap *h, void *out);

#endif
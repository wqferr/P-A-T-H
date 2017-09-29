#ifndef HEAP_H
#define HEAP_H 1

#include <stddef.h>
#include <stdbool.h>

#include "misc/hash.h"
#include "misc/compare.h"

/* Min heap structure */
typedef struct heap heap;

heap *heap_create(size_t elm_size);
void heap_destroy(heap *h);

void heap_map_indices(heap *h, hash_f hash, cmp_f comp);
void heap_ensure_capacity(heap *h, size_t cap);
size_t heap_get_size(const heap *h);
bool heap_is_empty(const heap *h);

bool heap_add(heap *h, const void *elm, float pr);
int heap_pop(heap *h, void *out);
bool heap_update(heap *h, const void *elm, float newpr);

#endif
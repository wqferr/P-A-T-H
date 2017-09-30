/*
 * William Quelho Ferreira
 */

#ifndef HEAP_H
#define HEAP_H 1

#include <stddef.h>
#include <stdbool.h>

#include "misc/hash.h"
#include "misc/compare.h"

/*
 * Min heap structure.
 */
typedef struct heap heap;

/*
 * Creates a heap whose elements are elm_size
 * bytes long.
 */
heap *heap_create(size_t elm_size);
/*
 * Frees memory allocated for a given heap.
 * If its elements were pointers to allocated memory, they
 * WILL NOT be freed.
 */
void heap_destroy(heap *h);

/*
 * Enables index mapping.
 * This is necessary to use the heap_update function, and only
 * applies to elements inserted after this function was called.
 */
void heap_map_indices(heap *h, hash_f hash, cmp_f comp);
/*
 * Guarantees the heap can hold cap elements without reallocating.
 */
void heap_ensure_capacity(heap *h, size_t cap);
/*
 * Returns the number of elements in the heap.
 */
size_t heap_get_size(const heap *h);
/*
 * Equivalent to
 * 		(heap_get_size(h) == 0)
 */
bool heap_is_empty(const heap *h);

/*
 * Inserts an element elm into the heap with priority pr.
 */
bool heap_add(heap *h, const void *elm, float pr);
/*
 * Writes the element with minimum priority value to out.
 * Returns the priority associated with it.
 * If the heap is empty, out is left untouched and 0 is returned.
 */
float heap_pop(heap *h, void *out);
/*
 * Updates the priority of a value.
 * Can only be called upon elements inserted after the call to
 * heap_map_indices.
 * The new priority value must be smaller than the current one.
 * Returns whether or not the operation was successful.
 */
bool heap_update(heap *h, const void *elm, float newpr);

#endif
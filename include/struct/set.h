/*
 * William Quelho Ferreira
 */

#ifndef SET_H
#define SET_H 1

#include <stddef.h>
#include <stdbool.h>

#include "misc/hash.h"
#include "misc/compare.h"


#define SET_INITIAL_CAP 20
#define SET_DEFAULT_LOAD_FACTOR 0.5f

/*
 * A hash set of elements.
 */
typedef struct set set;

/*
 * Creates a new set.
 * Its elements have elm_size bytes, and the hash and compare
 * functions receive references to its element type.
 */
set *set_create(size_t elm_size, hash_f hash, cmp_f compare);
/*
 * Frees memory allocated for a given set.
 * If its elements were pointers to allocated memory, they
 * WILL NOT be freed.
 */
void set_destroy(set *s);

/*
 * Sets the maximum ratio between the size and the internal array size.
 * Must be in the open interval ]0, 1[.
 * A lower value will generally decrease search time, but increase
 * memory allocation.
 * Use with caution.
 */
bool set_set_load_factor(set *s, float lf);
/*
 * Resizes the set internally such that it can hold at least size elements.
 * This is called automatically if an insertion makes the size exceed the
 * load factor.
 */
void set_ensure_capacity(set *s, size_t cap);
/*
 * Returns the number of elements in the set.
 */
size_t set_get_size(const set *s);
/*
 * Equivalent to
 * 		(set_get_size(s) == 0)
 */
bool set_is_empty(const set *s);

/*
 * Inserts a new element into the set.
 * If an equivalent element has already been inserted (i.e., and element e0
 * such that the comparison function with e and elm returns 0), the old value
 * is overwritten. In this case, if the old element was a pointer to allocated
 * memory, it WILL NOT be freed.
 */
bool set_insert(set *s, const void *elm);
/*
 * Removes the element from the set.
 * Returns whether or not the element was present.
 */
bool set_remove(set *s, const void *elm);
/*
 * Returns whether or not the map contains the given element.
 */
bool set_contains(const set *s, const void *elm);

#endif
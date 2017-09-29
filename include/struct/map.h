#ifndef MAP_H
#define MAP_H 1

#include <stddef.h>
#include <stdbool.h>

#include "misc/hash.h"
#include "misc/compare.h"

#define MAP_INITIAL_CAP 20
#define MAP_DEFAULT_LOAD_FACTOR 0.5f

/*
 * Associative array structure.
 */
typedef struct map map;

/*
 * Creates a new map.
 * Its keys have key_size bytes, its values have val_size bytes.
 * key_hash and key_comp apply their respective operations to
 * keys, and never values.
 */
map *map_create(
	size_t key_size, size_t val_size,
	hash_f key_hash, cmp_f key_comp);
/*
 * Frees memory allocated for a given map.
 * If its keys or values were pointers to allocated memory, they
 * WILL NOT be freed.
 */
void map_destroy(map *m);

/*
 * Sets the maximum ratio between the size and the internal array size.
 * Must be in the open interval ]0, 1[.
 * A lower value will generally decrease search time, but increase
 * memory allocation.
 * Use with caution.
 */
bool map_set_load_factor(map *m, float lf);
/*
 * Resizes the map internally such that it can hold at least size elements.
 * This is called automatically if an insertion makes the size exceed the
 * load factor.
 */
void map_ensure_capacity(map *m, size_t size);
/*
 * Returns the number of keys in the map.
 */
size_t map_get_size(const map *m);
/*
 * Equivalent to
 * 		(map_get_size(m) == 0)
 */
bool map_is_empty(const map *m);

/*
 * Returns whether or not there the key is present in the map.
 */
bool map_contains_key(const map *m, const void *key);
/*
 * Inserts the pair (key, value) into the map.
 * If key was already present, the value is replaced. In this case,
 * if the previous value was a pointer to allocated memory, it WILL NOT
 * be freed.
 */
bool map_put(map *m, const void *key, const void *val);
/*
 * If the map contains the given key, write its associated value to out
 * and return true.
 * Otherwise, out is left untouched and return false.
 */
bool map_get(map *m, const void *key, void *out);
/*
 * Removes the associated (key, value) pair from the map.
 * If key wasn't present in the map, return false. Otherwise, return true.
 */
bool map_remove(map *m, const void *key);

#endif
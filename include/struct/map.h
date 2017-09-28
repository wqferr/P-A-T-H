#ifndef MAP_H
#define MAP_H 1

#include <stddef.h>
#include <stdbool.h>

#include "misc/func.h"

#define MAP_INITIAL_CAP 20
#define MAP_DEFAULT_LOAD_FACTOR 0.5f

typedef struct map map;

map *map_create(
	size_t key_size, size_t val_size,
	hash_f key_hash, cmp_f key_comp);
void map_destroy(map *m);

bool map_set_load_factor(map *m, float lf);
void map_ensure_capacity(map *m, size_t cap);
size_t map_get_size(const map *m);
bool map_is_empty(const map *m);
bool map_is_full(const map *m);

bool map_contains_key(const map *m, const void *key);
bool map_put(map *m, const void *key, const void *val);
bool map_get(map *m, const void *key, void *out);
bool map_remove(map *m, const void *key);

#endif
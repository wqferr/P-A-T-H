#include "struct/set.h"

#include <stdlib.h>
#include <string.h>

#include "struct/map.h"

struct set {
	map *vals;
};


set *set_create(size_t elm_size, hash_f hash, cmp_f compare) {
	set *s = malloc(sizeof(*s));
	s->vals = map_create(elm_size, 0, hash, compare);

	map_set_load_factor(s->vals, SET_DEFAULT_LOAD_FACTOR);
	map_ensure_capacity(s->vals, SET_INITIAL_CAP);

	return s;
}

void set_destroy(set *s) {
	map_destroy(s->vals);
	free(s);
}

bool set_set_load_factor(set *s, float lf) {
	return map_set_load_factor(s->vals, lf);
}

void set_ensure_capacity(set *s, size_t cap) {
	map_ensure_capacity(s->vals, cap);
}

size_t set_get_size(const set *s) {
	return map_get_size(s->vals);
}

bool set_is_empty(const set *s) {
	return map_is_empty(s->vals);
}

bool set_insert(set *s, const void *elm) {
	return map_put(s->vals, elm, NULL);
}

bool set_remove(set *s, const void *elm) {
	return map_remove(s->vals, elm);
}

bool set_contains(const set *s, const void *elm) {
	return map_contains_key(s->vals, elm);
}
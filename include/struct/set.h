#ifndef SET_H
#define SET_H 1

#include <stddef.h>
#include <stdbool.h>

#include "misc/hash.h"
#include "misc/compare.h"


#define SET_INITIAL_CAP 20
#define SET_DEFAULT_LOAD_FACTOR 0.5f

typedef struct set set;

set *set_create(size_t elm_size, hash_f hash, cmp_f compare);
void set_destroy(set *s);

bool set_set_load_factor(set *s, float lf);
void set_ensure_capacity(set *s, size_t cap);
size_t set_get_size(const set *s);
bool set_is_empty(const set *s);
bool set_is_full(const set *s);

bool set_insert(set *s, const void *elm);
bool set_remove(set *s, const void *elm);
bool set_contains(const set *s, const void *elm);

#endif
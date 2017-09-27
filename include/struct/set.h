#ifndef SET_H
#define SET_H 1

#include <stddef.h>
#include <stdbool.h>


#define SET_INITIAL_CAP 50
#define SET_DEFAULT_LOAD_FACTOR 0.5

typedef struct set set;

typedef int (*hash_f)(const void *);
typedef int (*cmp_f)(const void *, const void *);

set *set_create(size_t elm_size, hash_f hash, cmp_f compare);
void set_destroy(set *s);

bool set_set_load_factor(set *s, float lf);
void set_ensure_capacity(set *s, size_t cap);
size_t set_get_size(const set *s);
bool set_is_empty(const set *s);
bool set_is_full(const set *s);

bool set_insert(set *s, const void *elm);
void *set_remove(set *s, const void *elm);
void set_delete(set *s, const void *elm);
bool set_contains(const set *s, const void *elm);

#endif
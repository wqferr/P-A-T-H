#include "struct/set.h"

#include <stdlib.h>
#include <string.h>

#define SET_IDX_FOUND 0
#define SET_FULL 1
#define SET_DUPL_INSERTION 2

#define SET_ELM_FOUND 0
#define SET_ELM_NOT_FOUND 1

typedef struct set_node set_node;

struct set {
	set_node *elm;
	float load_factor;

	size_t size;
	size_t cap;
	size_t elm_size;

	hash_f hash;
	cmp_f compare;
};

struct set_node {
	void *data;
	int hash;
	bool empty;
	bool removed;
};


void _set_resize(set *s, size_t cap);
int _set_find_empty_idx(
	const set *s,
	const void *elm,
	int hash,
	size_t *idx);
int _set_find_elm_idx(
	const set *s,
	const void *elm,
	int hash,
	size_t *idx);
void _set_create_node(set *s, size_t idx, const void *elm, int hash);


set *set_create(size_t elm_size, hash_f hash, cmp_f compare) {
	size_t i;
	set *s = malloc(sizeof(*s));
	s->load_factor = SET_DEFAULT_LOAD_FACTOR;
	s->size = 0;
	s->elm_size = elm_size;

	s->hash = hash;
	s->compare = compare;

	s->elm = malloc(SET_INITIAL_CAP * sizeof(*s->elm));
	for (i = 0; i < SET_INITIAL_CAP; i++) {
		s->elm[i].empty = true;
		s->elm[i].removed = false;
		s->elm[i].data = NULL;
		s->elm[i].hash = -1;
	}
	s->cap = SET_INITIAL_CAP;

	return s;
}

void set_destroy(set *s) {
	size_t i;
	for (i = 0; i < s->cap; i++) {
		if (!s->elm[i].empty) {
			free(s->elm[i].data);
		}
	}
	free(s->elm);
	free(s);
}

bool set_set_load_factor(set *s, float lf) {
	if (lf <= 0 || lf > 1) {
		return false;
	}
	s->load_factor = lf;
	return true;
}

void set_ensure_capacity(set *s, size_t cap) {
	if (s->cap < cap) {
		_set_resize(s, cap);
	}
}

size_t set_get_size(const set *s) {
	return s->size;
}

bool set_is_empty(const set *s) {
	return s->size == 0;
}

bool set_is_full(const set *s) {
	return s->size == s->cap;
}

bool set_insert(set *s, const void *elm) {
	int hash = s->hash(elm);
	int idx_res;
	size_t idx;

	idx_res = _set_find_empty_idx(s, elm, hash, &idx);
	if (idx_res == SET_FULL) {
		return false;
	}
	if (idx_res == SET_DUPL_INSERTION) {
		memcpy(s->elm[idx].data, elm, s->elm_size);
	} else {
		s->size++;
		_set_create_node(s, idx, elm, hash);
	}

	return true;
}

void *set_remove(set *s, const void *elm) {
	size_t idx;
	int hash = s->hash(elm);
	int idx_res = _set_find_elm_idx(s, elm, hash, &idx);

	if (idx_res == SET_ELM_FOUND) {
		s->elm[idx].removed = true;
		s->size--;
		return s->elm[idx].data;
	}
	return NULL;
}

void set_delete(set *s, const void *elm) {
	free(set_remove(s, elm));
}

bool set_contains(const set *s, const void *elm) {
	int hash = s->hash(elm);
	size_t idx;
	return _set_find_elm_idx(s, elm, hash, &idx) == SET_ELM_FOUND;
}



void _set_resize(set *s, size_t cap) {
	set_node *aux;
	size_t old_cap = s->cap;
	size_t i, idx;

	if (cap < s->size) {
		return;
	}

	aux = malloc(old_cap * sizeof(*aux));
	memcpy(aux, s->elm, old_cap * sizeof(*aux));
	s->elm = realloc(s->elm, cap*sizeof(*s->elm));
	s->cap = cap;

	for (i = 0; i < s->cap; i++) {
		s->elm[i].empty = true;
		s->elm[i].removed = false;
		s->elm[i].data = NULL;
		s->elm[i].hash = -1;
	}
	for (i = 0; i < old_cap; i++) {
		if (!aux[i].empty && !aux[i].removed) {
			_set_find_empty_idx(s, aux[i].data, aux[i].hash, &idx);
			s->elm[idx] = aux[i];
		}
	}
	free(aux);
}

int _set_find_empty_idx(
	const set *s,
	const void *elm,
	int hash,
	size_t *idx) {

	size_t i;
	if (set_is_full(s)) {
		return SET_FULL;
	}
	i = ((hash % s->cap) + s->cap) % s->cap;

	while (!(s->elm[i].empty || s->elm[i].removed)) {
		if (s->elm[i].hash == hash && s->compare(elm, s->elm[i].data) == 0) {
			*idx = i;
			return SET_DUPL_INSERTION;
		}
		i = (i+1) % s->cap;
	}
	*idx = i;
	return SET_IDX_FOUND;
}

int _set_find_elm_idx(
	const set *s,
	const void *elm,
	int hash,
	size_t *idx) {

	size_t i;
	size_t start;

	start = ((hash % s->cap) + s->cap) % s->cap;
	i = start;
	do {
		if (!(s->elm[i].empty || s->elm[i].removed)) {
			if (s->elm[i].hash == hash
				&& s->compare(s->elm[i].data, elm) == 0) {

				*idx = i;
				return SET_ELM_FOUND;
			}
		}
		i = (i+1) % s->cap;
	} while (i != start);
	return SET_ELM_NOT_FOUND;
}

void _set_create_node(set *s, size_t idx, const void *elm, int hash) {
	s->elm[idx].data = malloc(s->elm_size);
	memcpy(s->elm[idx].data, elm, s->elm_size);
	s->elm[idx].empty = false;
	s->elm[idx].removed = false;
	s->elm[idx].hash = hash;

	if (((float) s->size / s->cap) > s->load_factor) {
		_set_resize(s, 2*s->cap);
	}
}
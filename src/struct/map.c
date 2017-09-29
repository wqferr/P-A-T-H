#include "struct/map.h"

#include <stdlib.h>
#include <string.h>

#define MAP_IDX_FOUND 0
#define MAP_DUPL_INSERTION 1

#define MAP_ELM_FOUND 0
#define MAP_ELM_NOT_FOUND 1

typedef struct map_node map_node;

struct map {
	map_node *elm;
	float load_factor;

	size_t size;
	size_t cap;
	size_t key_size;
	size_t val_size;

	hash_f key_hash;
	cmp_f key_compare;
};

struct map_node {
	void *key;
	void *val;
	int32_t hash;
	bool empty;
	bool removed;
};


void _map_reset_arr(map *m);
void _map_resize(map *m, size_t cap);
int _map_find_empty_idx(
	const map *m,
	const void *elm,
	int32_t hash,
	size_t *idx);
int _map_find_key_idx(
	const map *m,
	const void *elm,
	int32_t hash,
	size_t *idx);
void _map_create_node(
	map *m,
	size_t idx,
	const void *key, const void *val,
	int32_t hash);


map *map_create(
	size_t key_size, size_t val_size,
	hash_f key_hash, cmp_f key_compare) {

	map *m = malloc(sizeof(*m));
	m->load_factor = MAP_DEFAULT_LOAD_FACTOR;
	m->size = 0;
	m->key_size = key_size;
	m->val_size = val_size;

	m->key_hash = key_hash;
	m->key_compare = key_compare;

	m->elm = malloc(MAP_INITIAL_CAP * sizeof(*m->elm));
	m->cap = MAP_INITIAL_CAP;
	_map_reset_arr(m);

	return m;
}

void map_destroy(map *m) {
	size_t i;
	for (i = 0; i < m->cap; i++) {
		if (!m->elm[i].empty && !m->elm[i].removed) {
			free(m->elm[i].key);
			free(m->elm[i].val);
		}
	}
	free(m->elm);
	free(m);
}

bool map_set_load_factor(map *m, float lf) {
	if (lf <= 0 || lf > 1) {
		return false;
	}
	m->load_factor = lf;
	return true;
}

void map_ensure_capacity(map *m, size_t size) {
	size_t cap = (size / m->load_factor) + 1;
	if (m->cap < cap) {
		_map_resize(m, cap);
	}
}

size_t map_get_size(const map *m) {
	return m->size;
}

bool map_is_empty(const map *m) {
	return m->size == 0;
}

bool map_put(map *m, const void *key, const void *val) {
	int32_t hash = m->key_hash(key);
	int idx_res;
	size_t idx;

	idx_res = _map_find_empty_idx(m, key, hash, &idx);
	if (idx_res == MAP_DUPL_INSERTION) {
		memcpy(m->elm[idx].key, key, m->key_size);
		memcpy(m->elm[idx].val, val, m->val_size);
		m->elm[idx].hash = hash;
	} else {
		m->size++;
		_map_create_node(m, idx, key, val, hash);
	}

	return true;
}

bool map_remove(map *m, const void *key) {
	size_t idx;
	int32_t hash = m->key_hash(key);
	int idx_res = _map_find_key_idx(m, key, hash, &idx);

	if (idx_res == MAP_ELM_FOUND) {
		m->elm[idx].removed = true;
		m->size--;
		free(m->elm[idx].key);
		free(m->elm[idx].val);
		return true;
	}
	return false;
}

bool map_get(map *m, const void *key, void *out) {
	size_t idx;
	int32_t hash = m->key_hash(key);
	int idx_res = _map_find_key_idx(m, key, hash, &idx);

	if (idx_res == MAP_ELM_FOUND) {
		memcpy(out, m->elm[idx].val, m->val_size);
		return true;
	}
	return false;
}

bool map_contains_key(const map *m, const void *elm) {
	int32_t hash = m->key_hash(elm);
	size_t idx;
	return _map_find_key_idx(m, elm, hash, &idx) == MAP_ELM_FOUND;
}



void _map_reset_arr(map *m) {
	size_t i;
	for (i = 0; i < m->cap; i++) {
		m->elm[i].empty = true;
		m->elm[i].removed = false;
		m->elm[i].key = NULL;
		m->elm[i].val = NULL;
		m->elm[i].hash = 0;
	}
}

void _map_resize(map *m, size_t cap) {
	map_node *aux;
	size_t old_cap = m->cap;
	size_t i, idx;

	if (cap < m->size) {
		return;
	}

	aux = malloc(old_cap * sizeof(*aux));
	memcpy(aux, m->elm, old_cap * sizeof(*aux));
	m->elm = realloc(m->elm, cap*sizeof(*m->elm));
	m->cap = cap;

	_map_reset_arr(m);
	for (i = 0; i < old_cap; i++) {
		if (!aux[i].empty && !aux[i].removed) {
			_map_find_empty_idx(m, aux[i].key, aux[i].hash, &idx);
			m->elm[idx] = aux[i];
		}
	}
	free(aux);
}

int _map_find_empty_idx(
	const map *m,
	const void *elm,
	int32_t hash,
	size_t *idx) {

	size_t i;
	i = ((hash % m->cap) + m->cap) % m->cap;

	while (!(m->elm[i].empty || m->elm[i].removed)) {
		if (m->elm[i].hash == hash && m->key_compare(elm, m->elm[i].key) == 0) {
			*idx = i;
			return MAP_DUPL_INSERTION;
		}
		i = (i+1) % m->cap;
	}
	*idx = i;
	return MAP_IDX_FOUND;
}

int _map_find_key_idx(
	const map *m,
	const void *elm,
	int32_t hash,
	size_t *idx) {

	size_t i;
	size_t start;

	start = ((hash % m->cap) + m->cap) % m->cap;
	i = start;
	do {
		if (!(m->elm[i].empty || m->elm[i].removed)) {
			if (m->elm[i].hash == hash
				&& m->key_compare(m->elm[i].key, elm) == 0) {

				*idx = i;
				return MAP_ELM_FOUND;
			}
		}
		i = (i+1) % m->cap;
	} while (i != start);
	return MAP_ELM_NOT_FOUND;
}

void _map_create_node(
	map *m,
	size_t idx,
	const void *key, const void *val,
	int32_t hash) {

	m->elm[idx].key = malloc(m->key_size);
	memcpy(m->elm[idx].key, key, m->key_size);
	m->elm[idx].val = malloc(m->val_size);
	memcpy(m->elm[idx].val, val, m->val_size);

	m->elm[idx].empty = false;
	m->elm[idx].removed = false;
	m->elm[idx].hash = hash;

	if (((float) m->size / m->cap) > m->load_factor) {
		_map_resize(m, 2*m->cap);
	}
}
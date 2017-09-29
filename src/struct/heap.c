#include "struct/heap.h"

#include <stdlib.h>
#include <string.h>

#include "struct/map.h"

typedef struct heap_node heap_node;

#define LCHILD(p) (2*p+1)
#define RCHILD(p) (2*p+2)
#define PARENT(c) ((c-1)/2)

#define _heap_cmp(h, i, j) (\
	(h)->elm[(i)].priority < (h)->elm[(j)].priority)

struct heap {
	heap_node *elm;
	size_t size;
	size_t cap;
	size_t elm_size;

	map *indices;
};

struct heap_node {
	void *data;
	float priority;
};



void _heap_resize(heap *h, size_t cap);
void _heap_try_shrink(heap *h);
void _heap_add_node(heap *h, const void *elm, float pr);
void _heap_swp(heap *h, size_t i, size_t j);
void _heap_fix_up(heap *h, size_t p);
void _heap_fix_down(heap *h, size_t c);

heap *heap_create(size_t elm_size) {
	heap *h = malloc(sizeof(*h));
	h->elm = NULL;
	h->indices = NULL;
	h->size = 0;
	h->elm_size = elm_size;
	_heap_resize(h, 10);

	return h;
}

void heap_destroy(heap *h) {
	while (!heap_is_empty(h)) {
		heap_pop(h, NULL);
	}
	if (h->indices != NULL) {
		map_destroy(h->indices);	
	}
	free(h->elm);
	free(h);
}

void heap_map_indices(heap *h, hash_f hash, cmp_f comp) {
	h->indices = map_create(
		h->elm_size, sizeof(size_t),
		hash, comp);
}


void heap_ensure_capacity(heap *h, size_t cap) {
	if (h->cap < cap) {

		_heap_resize(h, cap);
	}
}

size_t heap_get_size(const heap *h) {
	return h->size;
}

bool heap_is_empty(const heap *h) {
	return h->size == 0;
}


bool heap_add(heap *h, const void *elm, float pr) {
	if (elm == NULL) {
		return false;
	}
	if (h->size == h->cap) {
		heap_ensure_capacity(h, 2*h->cap);
	}
	_heap_add_node(h, elm, pr);
	_heap_fix_up(h, h->size);
	h->size++;
	return true;
}

float heap_pop(heap *h, void *out) {
	int pr;
	if (h->size == 0) {
		return 0;
	}
	if (out != NULL) {
		memcpy(out, h->elm[0].data, h->elm_size);
	}
	pr = h->elm[0].priority;
	free(h->elm[0].data);

	h->size--;
	h->elm[0] = h->elm[h->size];

	_heap_fix_down(h, 0);
	_heap_try_shrink(h);
	return pr;
}

bool heap_update(heap *h, const void *elm, float newpr) {
	size_t idx;
	if (h->indices == NULL || !map_get(h->indices, elm, &idx)) {
		return false;
	}
	if (newpr > h->elm[idx].priority) {
		return false;
	}
	h->elm[idx].priority = newpr;
	_heap_fix_up(h, idx);
	return true;
}


void _heap_resize(heap *h, size_t cap) {
	h->cap = cap;
	h->elm = realloc(h->elm, cap*sizeof(*h->elm));
}

void _heap_try_shrink(heap *h) {
	size_t poss_new_cap = 2 * h->cap / 3;
	if (h->size < poss_new_cap) {
		_heap_resize(h, poss_new_cap);
	}
}

void _heap_add_node(heap *h, const void *elm, float pr) {
	heap_node n;
	n.data = malloc(h->elm_size);
	memcpy(n.data, elm, h->elm_size);
	n.priority = pr;

	h->elm[h->size] = n;
}

void _heap_swp(heap *h, size_t i, size_t j) {
	heap_node n = h->elm[i];
	h->elm[i] = h->elm[j];
	h->elm[j] = n;
	if (h->indices != NULL) {
		map_put(h->indices, h->elm[i].data, &i);
		map_put(h->indices, h->elm[j].data, &j);
	}
}

void _heap_fix_up(heap *h, size_t p) {
	size_t c;
	while((c=p) != 0 && _heap_cmp(h, c, (p=PARENT(c)))) {
		_heap_swp(h, c, p);
	}
	if (h->indices != NULL) {
		map_put(h->indices, h->elm[c].data, &c);	
	}
}

void _heap_fix_down(heap *h, size_t c) {
	size_t new_parent = c;
	size_t lchild = LCHILD(c);
	size_t rchild = RCHILD(c);
	if (lchild < h->size && _heap_cmp(h, lchild, new_parent)) {
		new_parent = lchild;
	}
	if (rchild < h->size && _heap_cmp(h, rchild, new_parent)) {
		new_parent = rchild;
	}

	if (new_parent != c) {
		_heap_swp(h, new_parent, c);
		_heap_fix_down(h, new_parent);
	}
}
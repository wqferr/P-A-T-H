#include "struct/maxheap.h"

#include <stdlib.h>
#include <string.h>

typedef struct heap_node heap_node;

#define LCHILD(p) (2*p+1)
#define RCHILD(p) (2*p+2)
#define PARENT(c) (c/2)

#define _heap_gt(h, i, j) (\
	(h)->elm[(i)].priority > (h)->elm[(j)].priority)

struct heap {
	heap_node *elm;
	size_t size;
	size_t cap;
	size_t elm_size;
};

struct heap_node {
	void *data;
	int priority;
};



void _heap_resize(heap *h, size_t cap);
void _heap_try_shrink(heap *h);
void _heap_add_node(heap *h, const void *elm, int pr);
void _heap_swp(heap *h, size_t i, size_t j);

heap *heap_create(size_t elm_size) {
	heap *h = malloc(sizeof(*h));
	h->elm = NULL;
	h->size = 0;
	h->elm_size = elm_size;
	_heap_resize(h, 10);

	return h;
}

void heap_destroy(heap *h) {
	while (!heap_is_empty(h)) {
		heap_pop(h, NULL);
	}
	free(h->elm);
	free(h);
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


bool heap_add(heap *h, const void *elm, int pr) {
	size_t cur, par;
	if (elm == NULL) {
		return false;
	}
	if (h->size == h->cap) {
		heap_ensure_capacity(h, 2*h->cap);
	}
	_heap_add_node(h, elm, pr);

	cur = h->size;
	while(cur != 0
			&& _heap_gt(h, cur, (par=PARENT(cur)))) {
		_heap_swp(h, cur, par);
		cur = par;
	}

	h->size++;
	return true;
}

int heap_pop(heap *h, void *out) {
	int pr;
	size_t cur, max_child;
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

	cur = -1;
	max_child = 0;
	while (max_child != cur && (cur = max_child) < h->size) {
		if (LCHILD(cur) < h->size && _heap_gt(h, LCHILD(cur), max_child)) {
			max_child = LCHILD(cur);
		}
		if (RCHILD(cur) < h->size && _heap_gt(h, RCHILD(cur), max_child)) {
			max_child = RCHILD(cur);
		}
		if (max_child != cur) {
			_heap_swp(h, cur, max_child);
		}
	}

	_heap_try_shrink(h);
	return pr;
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

void _heap_add_node(heap *h, const void *elm, int pr) {
	heap_node n;
	n.data = malloc(h->elm_size);
	memcpy(n.data, elm, h->elm_size);
	n.priority = pr;

	h->elm[h->size] = n;
}

void _max_heapify(heap *h, size_t c) {
	size_t p;
	while(c != 0 && _heap_gt(h, c, (p=PARENT(c)))) {
		_heap_swp(h, c, p);
		c = p;
	}
}

void _heap_swp(heap *h, size_t i, size_t j) {
	heap_node n = h->elm[i];
	h->elm[i] = h->elm[j];
	h->elm[j] = n;
}
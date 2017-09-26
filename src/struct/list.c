#include "struct/list.h"

#include <stdlib.h>
#include <string.h>

struct list {
	size_t elm_size;
	size_t len;
	size_t cap;
	void *elm;
};


void _list_resize(list *l, size_t min_cap);
void _list_try_shrink(list *l);
inline void _list_set_elm(list *l, size_t i, const void *elm_ref);
inline void _list_mov_elm(list *l, size_t i, size_t j);
inline void _list_elm_cpy(void *out, const list *l, size_t i);


list *list_create(size_t elem_size) {
	list *l = malloc(sizeof(*l));
	l->elm_size = elem_size;
	l->len = 0;
	l->elm = NULL;
	_list_resize(l, 1);

	return l;
}

void list_destroy(list *l) {
	free(l->elm);
	free(l);
}

void list_ensure_capacity(list *l, size_t cap) {
	if (cap > l->cap) {
		_list_resize(l, cap);
	}
}


size_t list_get_length(const list *l) {
	return l->len;
}

bool list_is_empty(const list *l) {
	return l->len == 0;
}


void list_push_front(list *l, const void *elm_ref) {
	size_t i;
	if (l->cap == l->len) {
		_list_resize(l, 2*l->cap);
	}
	/* Shift array */
	for (i = l->len; i > 0; i--) {
		_list_mov_elm(l, i, i-1);
	}
	_list_set_elm(l, 0, elm_ref);
	l->len++;
}

void list_push_back(list *l, const void *elm_ref) {
	if (l->cap == l->len) {
		_list_resize(l, 2*l->cap);
	}
	_list_set_elm(l, l->len, elm_ref);
	l->len++;
}

bool list_pop_front(list *l, void *out) {
	size_t i;
	if (list_is_empty(l)) {
		return false;
	}
	l->len--;
	_list_elm_cpy(out, l, 0);
	for (i = 0; i < l->len; i++) {
		_list_mov_elm(l, i, i+1);
	}
	_list_try_shrink(l);
	return true;
}

bool list_pop_back(list *l, void *out) {
	if (list_is_empty(l)) {
		return false;
	}
	l->len--;
	_list_elm_cpy(out, l, l->len);
	_list_try_shrink(l);
	return true;
}

void list_foreach(list *l, void (*f)(void *)) {
	size_t i;
	for (i = 0; i < l->len; i++) {
		f(l->elm + i*l->elm_size);
	}
}



void _list_resize(list *l, size_t cap) {
	l->cap = cap;
	l->elm = realloc(l->elm, cap * l->elm_size);
}

void _list_try_shrink(list *l) {
	size_t poss_new_cap = 2 * l->cap/3;
	if (l->len < poss_new_cap) {
		_list_resize(l, poss_new_cap);
	}
}

void _list_set_elm(list *l, size_t i, const void *elm_ref) {
	memcpy(
		l->elm + i*l->elm_size,
		elm_ref,
		l->elm_size);
}

void _list_mov_elm(list *l, size_t i, size_t j) {
	_list_set_elm(
		l,
		i,
		l->elm + j*l->elm_size);
}

void _list_elm_cpy(void *out, const list *l, size_t i) {
	memcpy(
		out,
		l->elm + i*l->elm_size,
		l->elm_size);
}
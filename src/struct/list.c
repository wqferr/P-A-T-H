#include "struct/list.h"

#include <stdlib.h>
#include <string.h>

typedef struct list_node list_node;

struct list {
	list_node *head;
	size_t size;
	size_t elm_size;
};

struct list_node {
	void *data;

	list_node *next;
	list_node *prev;
};


void _list_create_head_node(list *l);
void _list_create_node(list *l, list_node *prev_node, const void *elm);
void _list_delete_node(list *l, list_node *node, void *out);

list *list_create(size_t elm_size) {
	list *l = malloc(sizeof(*l));
	l->size = 0;
	l->elm_size = elm_size;

	_list_create_head_node(l);
	return l;
}

void list_destroy(list *l) {
	void *buf = malloc(l->elm_size);
	while (!list_is_empty(l)) {
		list_pop_front(l, buf);
	}
	free(buf);
	free(l->head);
	free(l);
}

size_t list_get_size(const list *l) {
	return l->size;
}

bool list_is_empty(const list *l) {
	return l->size == 0;
}

void list_push_front(list *l, const void *elm) {
	_list_create_node(l, l->head, elm);
}

void list_push_back(list *l, const void *elm) {
	_list_create_node(l, l->head->prev, elm);
}

bool list_pop_front(list *l, void *out) {
	if (list_is_empty(l)) {
		return false;
	}
	_list_delete_node(l, l->head->next, out);
	return true;
}

bool list_pop_back(list *l, void *out) {
	if (list_is_empty(l)) {
		return false;
	}
	_list_delete_node(l, l->head->prev, out);
	return true;
}

void list_for_each(list *l, void (*f)(void *elm, void *arg), void *arg) {
	list_node *node = l->head->next;
	while (node != l->head) {
		f(node->data, arg);
		node = node->next;
	}
}


void _list_create_head_node(list *l) {
	l->head = malloc(sizeof(*l->head));
	l->head->data = NULL;
	l->head->next = l->head;
	l->head->prev = l->head;
}

void _list_create_node(list *l, list_node *prev_node, const void *elm) {
	list_node *new = malloc(sizeof(*new));
	new->data = malloc(l->elm_size);
	memcpy(new->data, elm, l->elm_size);
	new->prev = prev_node;
	new->next = prev_node->next;
	new->prev->next = new;
	new->next->prev = new;

	l->size++;
}

void _list_delete_node(list *l, list_node *node, void *out) {
	memcpy(out, node->data, l->elm_size);
	node->prev->next = node->next;
	node->next->prev = node->prev;
	free(node->data);
	free(node);

	l->size--;
}
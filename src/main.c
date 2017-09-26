#include <stdio.h>

#include "struct/list.h"

void print_int(void *ref) {
	int *i = (int *) ref;
	printf("%d\n", *i);
}

int main(int argc, char const *argv[]) {
	list *l = list_create(sizeof(int));
	int x;

	list_ensure_capacity(l, 10);
	for (x = 0; x < 10; x++) {
		list_push_back(l, &x);
		list_push_front(l, &x);
	}

	list_foreach(l, &print_int);

	printf("---\n");

	while (!list_is_empty(l)) {
		list_pop_back(l, &x);
		printf("%zu - %d\n", list_get_length(l), x);
	}

	list_destroy(l);
	return 0;
}
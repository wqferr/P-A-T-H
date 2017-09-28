#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "struct/vec2.h"
#include "struct/set.h"
#include "struct/minheap.h"

#define N 20

int32_t vec2_hash(const void *ref) {
	vec2 v = *((vec2 *) ref);
	return 13*v.x + v.y;
}

int vec2_compare(const void *ref1, const void *ref2) {
	vec2 u = *((vec2 *) ref1);
	vec2 v = *((vec2 *) ref2);
	if (u.x == v.x && u.y == v.y) {
		return 0;
	}
	return 1;
}

int32_t int_hash(const void *ref) {
	return *((int *) ref);
}

int int_compare(const void *ref1, const void *ref2) {
	int i = *((int *) ref1);
	int j = *((int *) ref2);
	return (i-j);
}

int main(int argc, char const *argv[]) {
	heap *h = heap_create(sizeof(int));
	set *s = set_create(sizeof(int), &int_hash, &int_compare);
	int i, x, max = -1;

	heap_map_indices(h, &int_hash, &int_compare);

	srand(time(NULL));
	for (i = 0; i < N; i++) {
		do {
			x = rand() % (2*N);
		} while (set_contains(s, &x));
		heap_add(h, &x, x);
		set_insert(s, &x);
		printf("%d\n", x);
		if (x > max) {
			max = x;
		}
	}

	printf("---\n");

	heap_update(h, &max, -1);
	while (!heap_is_empty(h)) {
		heap_pop(h, &x);
		printf("%d\n", x);
	}

	heap_destroy(h);
	set_destroy(s);
	return 0;
}
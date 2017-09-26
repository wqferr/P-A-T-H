#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "struct/list.h"
#include "struct/vec2.h"
#include "struct/maxheap.h"

int main(int argc, char const *argv[]) {
	heap *h = heap_create(sizeof(int));
	int x = 0;
	int i;
	srand(time(NULL));

	for (i = 0; i < 100; i++) {
		x = rand() % 1000;
		heap_add(h, &x, x);
	}

	while (!heap_is_empty(h)) {
		printf("%d ", heap_pop(h, &x));
		printf("%d\n", x);
	}

	heap_destroy(h);
	return 0;
}
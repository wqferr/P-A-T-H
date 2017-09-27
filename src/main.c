#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "struct/list.h"
#include "struct/vec2.h"
#include "struct/minheap.h"

#define N 10

int main(int argc, char const *argv[]) {
	heap *h = heap_create(sizeof(int));
	int x = 0, y;
	int i;

	srand(time(NULL));

	for (i = 0; i < N; i++) {
		x = rand() % 1000;
		heap_add(h, &x, x);
		printf("%d\n", x);
	}

	printf("----\n");
	for (i = 0; i < N; i++) {
		heap_pop(h, &x);
		printf("%d\n", x);
	}

	heap_destroy(h);
	return 0;
}
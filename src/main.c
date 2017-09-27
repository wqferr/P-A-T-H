#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "struct/list.h"

#define N 20

void int_print(void *ref) {
	printf("%d\n", *((int *) ref));
}

int main(int argc, char const *argv[]) {
	list *l = list_create(sizeof(int));
	int x = 5;
	int i;

	srand(time(NULL));
	for (i = 0; i < N; i++) {
		x = rand() % (N/2);
		printf("%d\n", x);
		list_push_front(l, &x);
	}

	printf("---\n");

	list_for_each(l, &int_print);
	
	list_destroy(l);
	return 0;
}
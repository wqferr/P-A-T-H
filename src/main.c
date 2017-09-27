#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "struct/set.h"

#define N 20

int int_hash(const void *ref) {
	return *((int *) ref);
}

int int_compare(const void *ref1, const void *ref2) {
	int i = *((int *) ref1);
	int j = *((int *) ref2);
	if (i < j) {
		return -1;
	} else if (i == j) {
		return 0;
	} else {
		return 1;
	}
}

int main(int argc, char const *argv[]) {
	set *s = set_create(sizeof(int), &int_hash, &int_compare);
	int x = 5;
	int i;

	srand(time(NULL));
	for (i = 0; i < N; i++) {
		x = rand() % (N/2);
		if (set_contains(s, &x)) {
			set_delete(s, &x);
		} else {
			set_insert(s, &x);	
		}
		printf("%d\t%zu\n", x, set_get_size(s));
	}
	printf("\n%zu distinct numbers\n", set_get_size(s));

	set_destroy(s);
	return 0;
}
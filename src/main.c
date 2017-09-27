#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "struct/set.h"

#define N 10

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
	int x;

	x = 5;
	set_insert(s, &x);
	set_insert(s, &x);
	x = 2;
	set_insert(s, &x);
	x = 55;
	set_insert(s, &x);
	x = 105;
	set_insert(s, &x);

	x = 5;
	printf("%d\n", set_contains(s, &x));
	set_remove(s, &x);
	printf("%d\n", set_contains(s, &x));
	x = 55;
	printf("%d\n", set_contains(s, &x));

	set_destroy(s);
	return 0;
}
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "struct/vec2.h"
#include "struct/set.h"

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
	set *s = set_create(sizeof(int), &int_hash, &int_compare);
	int i;

	i = 0;
	set_insert(s, &i);
	printf("%zu\t%d\n", set_get_size(s), set_contains(s, &i));

	i = 2;
	set_insert(s, &i);
	printf("%zu\t%d\n", set_get_size(s), set_contains(s, &i));

	i = 1;
	printf("%d\n", set_contains(s, &i));

	i = 2;
	set_remove(s, &i);
	printf("%zu\t%d\n", set_get_size(s), set_contains(s, &i));

	i = 0;
	printf("%d\n", set_contains(s, &i));

	set_destroy(s);
	return 0;
}
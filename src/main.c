#include <stdio.h>

#include "struct/list.h"
#include "struct/vec2.h"

void print_v2(vec2 v) {
	printf("(%hd, %hd)\n", v.x, v.y);
}

int main(int argc, char const *argv[]) {
	vec2 v = {1, 1};
	vec2 u = {2, 1};
	vec2 w = v;
	print_v2(v);
	printf("%f\n", vec2_norm(v));
	vec2_scale(&w, 2);
	print_v2(w);
	vec2_sub(&w, u);
	print_v2(w);

	return 0;
}
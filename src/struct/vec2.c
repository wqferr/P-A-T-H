/*
 * William Quelho Ferreira
 */

#include "struct/vec2.h"

float vec2_norm(vec2 v) {
	return sqrt((v.x*v.x) + (v.y*v.y));
}

float vec2_dist(vec2 v, vec2 u) {
	vec2_sub(&v, u);
	return vec2_norm(v);
}

void vec2_add(vec2 *v, vec2 u) {
	v->x += u.x;
	v->y += u.y;
}

void vec2_sub(vec2 *v, vec2 u) {
	v->x -= u.x;
	v->y -= u.y;
}

int vec2_comp(vec2 v, vec2 u) {
	return !(u.x == v.x && u.y == v.y);
}


int32_t vec2ref_hash(const void *ref) {
	vec2 v = *((const vec2 *) ref);
	return 31*v.x + v.y;
}

int vec2ref_comp(const void *ref1, const void *ref2) {
	vec2 u = *((const vec2 *) ref1);
	vec2 v = *((const vec2 *) ref2);
	if (u.x == v.x && u.y == v.y) {
		return 0;
	}
	return 1;
}
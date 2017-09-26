#include "struct/vec2.h"

inline vec2 vec2_create(coord x, coord y) {
	vec2 v = {x, y};
	return v;
}

inline float vec2_norm(vec2 v) {
	return sqrt((v.x*v.x) + (v.y*v.y));
}

inline float vec2_dist(vec2 v, vec2 u) {
	vec2_sub(&v, u);
	return vec2_norm(v);
}

inline void vec2_add(vec2 *v, vec2 u) {
	v->x += u.x;
	v->y += u.y;
}

inline void vec2_sub(vec2 *v, vec2 u) {
	v->x -= u.x;
	v->y -= u.y;
}

inline void vec2_scale(vec2 *v, float scale) {
	v->x = 0.5 + (v->x * scale);
	v->y = 0.5 + (v->y * scale);
}
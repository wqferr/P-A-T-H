#ifndef VEC2_H
#define VEC2_H 1

#include <math.h>
#include <stdint.h>

typedef int16_t coord;

typedef struct vec2 {
	coord x;
	coord y;
} vec2;

extern vec2 vec2_create(coord x, coord y);

extern float vec2_norm(vec2 v);
extern float vec2_dist(vec2 v, vec2 u);

extern void vec2_add(vec2 *v, vec2 u);
extern void vec2_sub(vec2 *v, vec2 u);
extern void vec2_scale(vec2 *v, float scale);

#endif
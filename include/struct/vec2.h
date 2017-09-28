#ifndef VEC2_H
#define VEC2_H 1

#include <math.h>
#include <stdint.h>

typedef int16_t coord;

typedef struct vec2 {
	coord x;
	coord y;
} vec2;

vec2 vec2_create(coord x, coord y);

float vec2_norm(vec2 v);
float vec2_dist(vec2 v, vec2 u);

void vec2_add(vec2 *v, vec2 u);
void vec2_sub(vec2 *v, vec2 u);

int vec2_comp(vec2 v, vec2 u);

int32_t vec2ref_hash(const void *ref);
int vec2ref_comp(const void *ref1, const void *ref2);

#endif
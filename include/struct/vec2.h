/*
 * William Quelho Ferreira
 */

#ifndef VEC2_H
#define VEC2_H 1

#include <math.h>
#include <stdint.h>

typedef int16_t coord;

typedef struct vec2 {
	coord x;
	coord y;
} vec2;

/* Returns the norm of the vector */
float vec2_norm(vec2 v);
/* Returns the norm of u-v */
float vec2_dist(vec2 v, vec2 u);

/* Add u into v */
void vec2_add(vec2 *v, vec2 u);
/* Add -u into v*/
void vec2_sub(vec2 *v, vec2 u);

/* Returns 0 if the vectors are equal. Nonzero otherwise. */
int vec2_comp(vec2 v, vec2 u);

/* Hashes a vector given its reference */
int32_t vec2ref_hash(const void *ref);
/* Compares two vectors given their references */
int vec2ref_comp(const void *ref1, const void *ref2);

#endif
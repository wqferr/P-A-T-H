/*
 * William Quelho Ferreira
 */

#ifndef HASH_H
#define HASH_H 1

#include <stdint.h>

/*
 * Hash function.
 * Must be consistent with a comparison function (see compare.h).
 * That is, given two elements, e1 and e2, and their
 * associated comparison function cmp
 * 		cmp(e1, e2) == 0  =>  hash(e1) == hash(e2)
 * Note that the relation is one way: if the hashes for two elements
 * are equal, that does NOT necessarily mean they are equivalent.
 */
typedef int32_t (*hash_f)(const void *);

#endif

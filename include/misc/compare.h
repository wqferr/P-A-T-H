#ifndef FUNC_H
#define FUNC_H 1

/*
 * Comparison function.
 * Must return 0 if and only if the elements the references point to
 * are equivalent.
 * If the elements have a notion of order, a positive return value
 * implies
 * 		*(ref1) > *(ref2)
 * Likewise, a negative return value implies
 * 		*(ref1) < *(ref2)
 */
typedef int (*cmp_f)(const void *ref1, const void *ref2);

#endif
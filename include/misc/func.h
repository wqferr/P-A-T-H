#ifndef FUNC_H
#define FUNC_H 1

#include <stdint.h>

typedef int32_t (*hash_f)(const void *);
typedef int (*cmp_f)(const void *, const void *);

#endif
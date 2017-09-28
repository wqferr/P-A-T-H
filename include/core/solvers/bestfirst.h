#ifndef MAZE_BEST_FIRST_H
#define MAZE_BEST_FIRST_H 1

#include "core/solver.h"

typedef float (*heuristic_f)(void *data, const maze *m, vec2 cur);

solver *solver_bestfirst_create(const maze *m, heuristic_f h);
void solver_bestfirst_destroy(solver *s);

#endif
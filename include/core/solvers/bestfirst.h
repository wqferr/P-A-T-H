#ifndef MAZE_BEST_FIRST_H
#define MAZE_BEST_FIRST_H 1

#include "core/maze.h"
#include "core/solver.h"
#include "misc/func.h"

solver *solver_bestfirst_create(const maze *m, heuristic_f h);
void solver_bestfirst_destroy(solver *s);

#endif
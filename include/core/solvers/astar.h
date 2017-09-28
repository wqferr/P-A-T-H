#ifndef A_STAR_H
#define A_STAR_H 1

#include "misc/func.h"
#include "core/solver.h"

solver *solver_astar_create(const maze *m, heuristic_f h);
void solver_astar_destroy(solver *s);

#endif
#ifndef A_H
#define A_H 1

#include "core/solver.h"

solver *solver_a_create(const maze *m, heuristic_f h);
void solver_a_destroy(solver *s);

#endif
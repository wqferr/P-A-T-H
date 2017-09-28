#ifndef MAZE_DFS_H
#define MAZE_DFS_H 1

#include "core/solver.h"

solver *solver_dfs_create(const maze *m);
void solver_dfs_destroy(solver *s);

#endif
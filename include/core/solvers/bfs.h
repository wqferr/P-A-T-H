#ifndef MAZE_BFS_H
#define MAZE_BFS_H 1

#include "core/solver.h"

solver *solver_bfs_create(const maze *m);
void solver_bfs_destroy(solver *s);

#endif
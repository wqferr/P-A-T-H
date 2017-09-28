#ifndef SOLVER_H
#define SOLVER_H 1

#include <stdbool.h>

#include "core/maze.h"
#include "struct/list.h"

typedef float (*heuristic_f)(void *data, const maze *m, vec2 cur);

/* 
 * Updates current position.
 * pos initially holds the current position of the solver, and must be
 * updated to the next cell in the graph.
 * Returns whether or not it might still be possible to solve the maze.
 */
typedef bool (*maze_move_f)(void *data, const maze *m, vec2 *pos);

/* Builds the path traced by the solver. */
typedef list *(*maze_path_f)(void *data, const maze *m);

typedef struct solver solver;

solver *solver_create(
	const maze *m,
	maze_move_f move,
	maze_path_f path);
void solver_destroy(solver *s);

void *solver_get_data(solver *s);
void solver_set_data(solver *s, void *data);
vec2 solver_get_cur_pos(const solver *s);

bool solver_step(solver *s);
bool solver_stuck(const solver *s);
bool solver_done(const solver *s);

bool solver_find(solver *s);
list *solver_get_path(solver *s);

#endif
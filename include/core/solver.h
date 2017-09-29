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
 * Returns false if and only if there is nowhere to go; true otherwise.
 */
typedef bool (*maze_move_f)(void *data, const maze *m, vec2 *pos);

/* Builds the path traced by the solver. */
typedef list *(*maze_path_f)(void *data, const maze *m);

typedef struct solver solver;

/*
 * Creates a solver for a given maze.
 * The movement of the solver will depend on the move function provided.
 * After the path has been found, the path function must return it in the form
 * of a list where successive calls to list_pop_front will yield the path in
 * sequence.
 * Some examples of famous algorithms have been implemented in
 * src/core/solvers/, which can be used as examples.
 */
solver *solver_create(
	const maze *m,
	maze_move_f move,
	maze_path_f path);
/*
 * Frees the memory allocated by solver_create.
 * The data pointer is left untouched, so if it was allocated,
 * it needs to be freed before solver_destroy.
 */
void solver_destroy(solver *s);

/* Returns the data pointer for the solver */
void *solver_get_data(solver *s);
/*
 * Sets the data pointer for the solver.
 * This can be anything the solving algorithm might need.
 */
void solver_set_data(solver *s, void *data);
/*
 * Gets the current position of the solver in the maze.
 */
vec2 solver_get_cur_pos(const solver *s);

/*
 * Perform one step following the solver's move function.
 * If it is stuck, returns false. Returns true otherwise.
 */
bool solver_step(solver *s);
/*
 * Returns true if the solver's move function returned false.
 */
bool solver_is_stuck(const solver *s);
/*
 * Returns true if the solver has reached the exit.
 */
bool solver_done(const solver *s);

/*
 * Tries to find the exit to the maze.
 * Returns true if it succeeds, false if it got stuck.
 */
bool solver_find(solver *s);
/*
 * Returns a list where successive calls to list_pop_front will
 * yield the sequence of vec2 the algorithm found.
 * The first call will return the maze start, the second will return
 * the first position the solver moved to, and so on.
 */
list *solver_get_path(solver *s);

#endif
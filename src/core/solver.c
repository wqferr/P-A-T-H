/*
 * William Quelho Ferreira
 */

#include "core/solver.h"

#include <stdlib.h>

struct solver {
	const maze *m;
	void *data;
	vec2 cur_pos;
	bool stuck;

	maze_move_f move;
	maze_path_f path;
};

solver *solver_create(const maze *m, maze_move_f move, maze_path_f path) {
	solver *s = malloc(sizeof(*s));
	s->m = m;
	s->data = NULL;
	s->move = move;
	s->path = path;
	s->stuck = false;

	s->cur_pos = maze_get_start(m);
	return s;
}

void solver_destroy(solver *s) {
	free(s);
}

void *solver_get_data(solver *s) {
	return s->data;
}

void solver_set_data(solver *s, void *data) {
	s->data = data;
}

bool solver_step(solver *s) {
	if (s->stuck) {
		return false;
	}
	if (!s->move(s->data, s->m, &s->cur_pos)) {
		s->stuck = true;
	}
	return !s->stuck;
}

bool solver_is_stuck(const solver *s) {
	return s->stuck;
}

bool solver_done(const solver *s) {
	return vec2_comp(s->cur_pos, maze_get_end(s->m)) == 0;
}


bool solver_find(solver *s) {
	while (!solver_done(s)) {
		solver_step(s);
		if (solver_is_stuck(s)) {
			return false;
		}
	}
	return true;
}

list *solver_get_path(solver *s) {
	if (s->stuck) {
		return NULL;
	}
	return s->path(s->data, s->m);
}
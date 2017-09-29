#include "core/solvers/bestfirst.h"

#include <stdlib.h>

#include "struct/set.h"
#include "struct/map.h"
#include "struct/list.h"
#include "struct/heap.h"

typedef struct bestfirst_data {
	set *visited;
	heap *open;
	map *parent;

	heuristic_f heuristic;
} bestfirst_data;

void _solver_bestfirst_init(solver *s, heuristic_f h);
bool _solver_bestfirst_move(void *data, const maze *m, vec2 *pos);
list *_solver_bestfirst_path(void *data, const maze *m);

solver *solver_bestfirst_create(const maze *m, heuristic_f h) {
	solver *s = solver_create(
		m, &_solver_bestfirst_move, &_solver_bestfirst_path);
	_solver_bestfirst_init(s, h);
	return s;
}

void solver_bestfirst_destroy(solver *s) {
	bestfirst_data *bestf = (bestfirst_data *) solver_get_data(s);
	set_destroy(bestf->visited);
	heap_destroy(bestf->open);
	map_destroy(bestf->parent);
	free(bestf);
	solver_destroy(s);
}

void _solver_bestfirst_init(solver *s, heuristic_f h) {
	bestfirst_data *bestf = malloc(sizeof(*bestf));
	bestf->visited = set_create(sizeof(vec2), &vec2ref_hash, &vec2ref_comp);
	bestf->open = heap_create(sizeof(vec2));
	bestf->parent = map_create(
		sizeof(vec2), sizeof(vec2), &vec2ref_hash, &vec2ref_comp);
	bestf->heuristic = h;
	solver_set_data(s, bestf);
}

bool _solver_bestfirst_move(void *data, const maze *m, vec2 *pos) {
	bestfirst_data bestf = *((bestfirst_data *) data);
	list *neighbors;
	vec2 neigh;
	float h;

	set_insert(bestf.visited, pos);
	neighbors = maze_get_neighbors(m, *pos);
	while (!list_is_empty(neighbors)) {
		list_pop_front(neighbors, &neigh);
		if (!set_contains(bestf.visited, &neigh)) {
			map_put(bestf.parent, &neigh, pos);

			h = bestf.heuristic(data, m, neigh);
			heap_add(bestf.open, &neigh, h);
		}
	}
	list_destroy(neighbors);

	if (heap_is_empty(bestf.open)) {
		return false;
	}
	heap_pop(bestf.open, pos);
	return true;
}

list *_solver_bestfirst_path(void *data, const maze *m) {
	list *path = list_create(sizeof(vec2));
	vec2 cur = maze_get_end(m);
	bestfirst_data bestf = *((bestfirst_data *) data);

	while (vec2_comp(cur, maze_get_start(m)) != 0) {
		list_push_front(path, &cur);
		map_get(bestf.parent, &cur, &cur);
	}
	list_push_front(path, &cur);

	return path;
}
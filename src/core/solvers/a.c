/*
 * William Quelho Ferreira
 */

#include "core/solvers/a.h"

#include <float.h>
#include <stdlib.h>

#include "struct/set.h"
#include "struct/map.h"
#include "struct/list.h"
#include "struct/heap.h"

typedef struct a_data {
	set *visited;

	heap *open;
	map *parent;
	map *gscore;

	heuristic_f heur;
} a_data;

void _solver_a_init(solver *s, const maze *m, heuristic_f h);
bool _solver_a_move(void *data, const maze *m, vec2 *pos);
list *_solver_a_path(void *data, const maze *m);

float _solver_a_get_gscore(const a_data *a, vec2 pos);

solver *solver_a_create(const maze *m, heuristic_f h) {
	solver *s = solver_create(m, &_solver_a_move, &_solver_a_path);
	_solver_a_init(s, m, h);
	return s;
}

void solver_a_destroy(solver *s) {
	a_data *a = (a_data *) solver_get_data(s);
	set_destroy(a->visited);
	heap_destroy(a->open);
	map_destroy(a->parent);
	map_destroy(a->gscore);
	free(a);
	solver_destroy(s);
}

void _solver_a_init(solver *s, const maze *m, heuristic_f h) {
	vec2 start = maze_get_start(m);
	float g;
	a_data *data = malloc(sizeof(a_data));

	data->visited = set_create(sizeof(vec2), &vec2ref_hash, &vec2ref_comp);
	data->open = heap_create(sizeof(vec2));
	data->parent = map_create(
		sizeof(vec2), sizeof(vec2), &vec2ref_hash, &vec2ref_comp);

	data->gscore = map_create(
		sizeof(vec2), sizeof(float), &vec2ref_hash, &vec2ref_comp);
	data->heur = h;
	solver_set_data(s, data);

	g = 0;
	map_put(data->gscore, &start, &g);
}

bool _solver_a_move(void *data, const maze *m, vec2 *pos) {
	a_data a = *((a_data *) data);
	list *neighbors;
	vec2 neigh;
	float f, g;
	float cur_g = _solver_a_get_gscore(&a, *pos);

	set_insert(a.visited, pos);

	neighbors = maze_get_neighbors(m, *pos);
	while (!list_is_empty(neighbors)) {
		list_pop_front(neighbors, &neigh);
		if (!set_contains(a.visited, &neigh)) {
			g = cur_g + vec2_dist(*pos, neigh);
			if (g < _solver_a_get_gscore(&a, neigh)) {
				f = g + a.heur(data, m, *pos);

				map_put(a.parent, &neigh, pos);
				map_put(a.gscore, &neigh, &g);
				if (!heap_update(a.open, &neigh, f)) {
					heap_add(a.open, &neigh, f);	
				}
			}
		}
	}
	list_destroy(neighbors);

	if (heap_is_empty(a.open)) {
		return false;
	}
	heap_pop(a.open, pos);
	return true;
}

list *_solver_a_path(void *data, const maze *m) {
	list *path = list_create(sizeof(vec2));
	vec2 cur = maze_get_end(m);
	a_data a = *((a_data *) data);

	while (vec2_comp(cur, maze_get_start(m)) != 0) {
		list_push_front(path, &cur);
		map_get(a.parent, &cur, &cur);
	}
	list_push_front(path, &cur);

	return path;
}

float _solver_a_get_gscore(const a_data *data, vec2 pos) {
	float g = FLT_MAX;
	if (map_contains_key(data->gscore, &pos)) {
		map_get(data->gscore, &pos, &g);
	}
	return g;
}
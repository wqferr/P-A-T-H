/*
 * William Quelho Ferreira
 */

#include "core/solvers/astar.h"

#include <float.h>
#include <stdlib.h>

#include "struct/set.h"
#include "struct/map.h"
#include "struct/list.h"
#include "struct/heap.h"

typedef struct astar_data {
	set *visited;

	heap *open;
	map *parent;
	map *gscore;

	heuristic_f heur;
} astar_data;

void _solver_astar_init(solver *s, const maze *m, heuristic_f h);
bool _solver_astar_move(void *data, const maze *m, vec2 *pos);
list *_solver_astar_path(void *data, const maze *m);

float _solver_astar_get_gscore(const astar_data *astar, vec2 pos);

solver *solver_astar_create(const maze *m, heuristic_f h) {
	solver *s = solver_create(m, &_solver_astar_move, &_solver_astar_path);
	_solver_astar_init(s, m, h);
	return s;
}

void solver_astar_destroy(solver *s) {
	astar_data *astar = (astar_data *) solver_get_data(s);
	set_destroy(astar->visited);
	heap_destroy(astar->open);
	map_destroy(astar->parent);
	map_destroy(astar->gscore);
	free(astar);
	solver_destroy(s);
}

void _solver_astar_init(solver *s, const maze *m, heuristic_f h) {
	vec2 start = maze_get_start(m);
	float g;
	astar_data *data = malloc(sizeof(astar_data));

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

bool _solver_astar_move(void *data, const maze *m, vec2 *pos) {
	astar_data astar = *((astar_data *) data);
	list *neighbors;
	vec2 neigh;
	float f, g;
	float cur_g = _solver_astar_get_gscore(&astar, *pos);

	set_insert(astar.visited, pos);

	neighbors = maze_get_neighbors(m, *pos);
	while (!list_is_empty(neighbors)) {
		list_pop_front(neighbors, &neigh);
		if (!set_contains(astar.visited, &neigh)) {
			g = cur_g + vec2_dist(*pos, neigh);
			if (g < _solver_astar_get_gscore(&astar, neigh)) {
				f = g + astar.heur(data, m, *pos);

				map_put(astar.parent, &neigh, pos);
				map_put(astar.gscore, &neigh, &g);
				if (!heap_update(astar.open, &neigh, f)) {
					heap_add(astar.open, &neigh, f);	
				}
			}
		}
	}
	list_destroy(neighbors);

	if (heap_is_empty(astar.open)) {
		return false;
	}
	heap_pop(astar.open, pos);
	return true;
}

list *_solver_astar_path(void *data, const maze *m) {
	list *path = list_create(sizeof(vec2));
	vec2 cur = maze_get_end(m);
	astar_data astar = *((astar_data *) data);

	while (vec2_comp(cur, maze_get_start(m)) != 0) {
		list_push_front(path, &cur);
		map_get(astar.parent, &cur, &cur);
	}
	list_push_front(path, &cur);

	return path;
}

float _solver_astar_get_gscore(const astar_data *data, vec2 pos) {
	float g = FLT_MAX;
	if (map_contains_key(data->gscore, &pos)) {
		map_get(data->gscore, &pos, &g);
	}
	return g;
}
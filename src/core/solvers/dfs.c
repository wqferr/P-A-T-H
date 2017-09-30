/*
 * William Quelho Ferreira
 */

#include "core/solvers/dfs.h"

#include <stdlib.h>
#include <stdbool.h>

#include "struct/vec2.h"
#include "struct/list.h"
#include "struct/set.h"
#include "struct/map.h"

typedef struct dfs_data {
	set *visited;
	list *stack;
	map *parent;
} dfs_data;

void _solver_dfs_init(solver *s);
bool _solver_dfs_move(void *data, const maze *m, vec2 *pos);
list *_solver_dfs_path(void *data, const maze *m);

solver *solver_dfs_create(const maze *m) {
	solver *s = solver_create(m, &_solver_dfs_move, &_solver_dfs_path);
	_solver_dfs_init(s);
	return s;
}

void solver_dfs_destroy(solver *s) {
	dfs_data *data = (dfs_data *) solver_get_data(s);
	set_destroy(data->visited);
	list_destroy(data->stack);
	map_destroy(data->parent);
	free(data);
	solver_destroy(s);
}

bool _solver_dfs_move(void *data, const maze *m, vec2 *pos) {
	dfs_data dfs = *((dfs_data *) data);
	list *neighbors;
	vec2 neigh;

	set_insert(dfs.visited, pos);

	neighbors = maze_get_neighbors(m, *pos);
	while (!list_is_empty(neighbors)) {
		list_pop_front(neighbors, &neigh);
		if (!set_contains(dfs.visited, &neigh)) {
			map_put(dfs.parent, &neigh, pos);
			list_push_front(dfs.stack, &neigh);
		}
	}
	list_destroy(neighbors);

	if (list_is_empty(dfs.stack)) {
		return false;
	}
	list_pop_front(dfs.stack, pos);
	return true;
}

list *_solver_dfs_path(void *data, const maze *m) {
	list *path = list_create(sizeof(vec2));
	vec2 cur = maze_get_end(m);
	dfs_data dfs = *((dfs_data *) data);

	while (vec2_comp(cur, maze_get_start(m)) != 0) {
		list_push_front(path, &cur);
		map_get(dfs.parent, &cur, &cur);
	}
	list_push_front(path, &cur);

	return path;
}


void _solver_dfs_init(solver *s) {
	dfs_data *data = malloc(sizeof(*data));
	data->visited = set_create(sizeof(vec2), &vec2ref_hash, &vec2ref_comp);
	data->stack = list_create(sizeof(vec2));
	data->parent = map_create(
		sizeof(vec2), sizeof(vec2), &vec2ref_hash, &vec2ref_comp);
	solver_set_data(s, data);
}
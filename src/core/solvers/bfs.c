#include "core/solvers/bfs.h"

#include <stdlib.h>

#include "struct/set.h"
#include "struct/list.h"
#include "struct/map.h"

typedef struct bfs_data {
	set *visited;
	list *queue;
	map *parent;
} bfs_data;


void _solver_bfs_init(solver *s);
bool _solver_bfs_move(void *data, const maze *m, vec2 *pos);
list *_solver_bfs_path(void *data, const maze *m);


solver *solver_bfs_create(const maze *m) {
	solver *s = solver_create(m, &_solver_bfs_move, &_solver_bfs_path);
	_solver_bfs_init(s);
	return s;
}

void solver_bfs_destroy(solver *s) {
	bfs_data bfs = *((bfs_data *) solver_get_data(s));
	set_destroy(bfs.visited);
	list_destroy(bfs.queue);
	map_destroy(bfs.parent);
	free(solver_get_data(s));
	solver_destroy(s);
}

void _solver_bfs_init(solver *s) {
	bfs_data *data = malloc(sizeof(*data));
	data->visited = set_create(sizeof(vec2), &vec2ref_hash, &vec2ref_comp);
	data->queue = list_create(sizeof(vec2));
	data->parent = map_create(
		sizeof(vec2), sizeof(vec2), &vec2ref_hash, &vec2ref_comp);
	solver_set_data(s, data);
}

bool _solver_bfs_move(void *data, const maze *m, vec2 *pos) {
	bfs_data bfs = *((bfs_data *) data);
	list *neighbors;
	vec2 neigh;

	set_insert(bfs.visited, pos);
	neighbors = maze_get_neighbors(m, *pos);
	while (!list_is_empty(neighbors)) {
		list_pop_front(neighbors, &neigh);
		if (!map_contains_key(bfs.parent, &neigh)
			&& !set_contains(bfs.visited, &neigh)) {

			map_put(bfs.parent, &neigh, pos);
			list_push_back(bfs.queue, &neigh);
		}
	}
	list_destroy(neighbors);

	if (list_is_empty(bfs.queue)) {
		return false;
	}
	list_pop_front(bfs.queue, pos);
	return true;
}

list *_solver_bfs_path(void *data, const maze *m) {
	list *path = list_create(sizeof(vec2));
	vec2 cur = maze_get_end(m);
	bfs_data bfs = *((bfs_data *) data);

	while (vec2_comp(cur, maze_get_start(m)) != 0) {
		list_push_front(path, &cur);
		map_get(bfs.parent, &cur, &cur);
	}
	list_push_front(path, &cur);

	return path;
}
#include <stdio.h>
#include <stdlib.h>

#include "core/maze.h"
#include "core/solvers/bfs.h"
#include "core/solvers/dfs.h"
#include "core/solvers/bestfirst.h"
#include "core/solvers/astar.h"

#include "struct/set.h"


#define COORD_IN_PATH_CHR 'o'

void maze_print(const maze *m, set *vertices) {
	vec2 pos;
	size_t w, h;
	char c;

	maze_get_dimensions(m, &w, &h);
	for (pos.y = 0; pos.y < h; pos.y++) {
		for (pos.x = 0; pos.x < w; pos.x++) {
			c = (char) maze_get_tile(m, pos);
			if (c != TILE_START
				&& c != TILE_END
				&& set_contains(vertices, &pos)) {

				printf("%c", COORD_IN_PATH_CHR);
			} else {
				printf("%c", c);
			}
		}
		printf("\n");
	}
}

void vec2_print(vec2 v) {
	printf("(%hd, %hd)\n", v.x, v.y);
}

float heur_L1_dist(void *data, const maze *m, vec2 pos) {
	vec2 end = maze_get_end(m);
	return abs(end.x - pos.x) + abs(end.y - pos.y);
}

float heur_L2_dist(void *data, const maze *m, vec2 pos) {
	return vec2_dist(maze_get_end(m), pos);
}

int main(int argc, char const *argv[]) {
	maze *m = maze_read(stdin);
	solver *s = solver_astar_create(m, &heur_L2_dist);
	list *path;
	set *vertices;
	vec2 v, prev;
	float length = 0;

	solver_find(s);
	path = solver_get_path(s);

	if (path == NULL) {
		printf("No path to exit found\n");
	} else {
		vertices = set_create(sizeof(vec2), &vec2ref_hash, &vec2ref_comp);
		list_pop_front(path, &prev);
		while (!list_is_empty(path)) {
			list_pop_front(path, &v);
			vec2_print(v);
			set_insert(vertices, &v);

			length += vec2_dist(v, prev);
			prev = v;
		}
		list_destroy(path);
		printf("Path length: %.2f", length);
		printf("\n\n");

		maze_print(m, vertices);
		set_destroy(vertices);
	}

	solver_astar_destroy(s);
	maze_destroy(m);
	return 0;
}
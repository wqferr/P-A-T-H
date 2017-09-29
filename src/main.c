#include <stdio.h>
#include <stdlib.h>

#include "core/maze.h"
#include "core/solvers/bfs.h"
#include "core/solvers/dfs.h"
#include "core/solvers/bestfirst.h"
#include "core/solvers/astar.h"

#include "struct/set.h"


#define COORD_IN_PATH_CHR 'o'
#define POINTS_PER_LINE 3


void maze_print(FILE *outstream, const maze *m, set *in_path);
void path_print(FILE *outstream, const maze *m, list *path);
void vec2_print(FILE *outstream, vec2 v);

float heur_L1(void *data, const maze *m, vec2 pos);
float heur_L2(void *data, const maze *m, vec2 pos);


#define solver_alg_create(m) (solver_astar_create(m, &heur_L2))
#define solver_alg_destroy(s) (solver_astar_destroy(s))

int main(int argc, char const *argv[]) {
	maze *m = maze_read(stdin);
	solver *s = solver_alg_create(m);
	list *path;

	if (solver_find(s)) {
		path = solver_get_path(s);
		path_print(stdout, m, path);
		list_destroy(path);
	} else {
		printf("No path to exit found\n");
	}

	solver_alg_destroy(s);
	maze_destroy(m);
	return 0;
}


void vec2_print(FILE *outstream, vec2 v) {
	fputc('(', outstream);
	fprintf(outstream, "%-3hd, %-3hd", v.x, v.y);
	fputc(')', outstream);
}

void maze_print(FILE *outstream, const maze *m, set *in_path) {
	vec2 pos;
	size_t w, h;
	char c;

	maze_get_dimensions(m, &w, &h);
	for (pos.y = 0; pos.y < h; pos.y++) {
		for (pos.x = 0; pos.x < w; pos.x++) {
			c = (char) maze_get_tile(m, pos);
			if (c != TILE_START
				&& c != TILE_END
				&& set_contains(in_path, &pos)) {

				fprintf(outstream, "%c", COORD_IN_PATH_CHR);
			} else {
				fprintf(outstream, "%c", c);
			}
		}
		fprintf(outstream, "\n");
	}
}

void path_print(FILE *outstream, const maze *m, list *path) {
	set *in_path;
	float len = 0;
	vec2 u, v;

	in_path = set_create(sizeof(vec2), &vec2ref_hash, &vec2ref_comp);
	set_ensure_capacity(in_path, list_get_size(path));

	list_pop_front(path, &u);
	vec2_print(outstream, u);
	fputc('\t', outstream);

	while (!list_is_empty(path)) {
		list_pop_front(path, &v);
		set_insert(in_path, &v);

		fprintf(outstream, "->\t");
		vec2_print(outstream, v);

		if ((set_get_size(in_path) % POINTS_PER_LINE) == 0) {
			fprintf(outstream, "\n\t\t");
		} else {
			fputc('\t', outstream);
		}

		len += vec2_dist(u, v);
		u = v;
	}
	fprintf(outstream, "\n\nPath length: %.2f\n\n", len);
	maze_print(outstream, m, in_path);

	set_destroy(in_path);
}


float heur_L1(void *data, const maze *m, vec2 pos) {
	vec2 end = maze_get_end(m);
	return abs(end.x - pos.x) + abs(end.y - pos.y);
}

float heur_L2(void *data, const maze *m, vec2 pos) {
	return vec2_dist(maze_get_end(m), pos);
}
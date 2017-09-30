/*
 * William Quelho Ferreira
 */

#include "core/maze.h"

#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>

#include "struct/set.h"

#define N_NEIGHBORS 8

struct maze {
	set *walls;
	size_t width;
	size_t height;

	vec2 start;
	vec2 end;
};


char _chr_read_non_whitespace(FILE *instream);

maze *maze_read(FILE *instream) {
	maze *m = malloc(sizeof(*m));
	size_t w, h;
	vec2 pos;
	char c;

	fscanf(instream, "%lu %lu", &h, &w);
	m->walls = set_create(sizeof(vec2), &vec2ref_hash, &vec2ref_comp);

	for (pos.y = 0; pos.y < h; pos.y++) {
		for (pos.x = 0; pos.x < w; pos.x++) {
			c = _chr_read_non_whitespace(instream);
			switch (c) {
				case TILE_WALL:
					set_insert(m->walls, &pos);
					break;
				case TILE_START:
					m->start = pos;
					break;
				case TILE_END:
					m->end = pos;
					break;
			}
		}
	}
	m->width = w;
	m->height = h;
	return m;
}

void maze_destroy(maze *m) {
	set_destroy(m->walls);
	free(m);
}

void maze_get_dimensions(const maze *m, size_t *w, size_t *h) {
	*w = m->width;
	*h = m->height;
}

vec2 maze_get_start(const maze *m) {
	return m->start;
}

vec2 maze_get_end(const maze *m) {
	return m->end;
}

tile maze_get_tile(const maze *m, vec2 pos) {
	if (pos.x < 0 || pos.y < 0 || pos.x >= m->width || pos.y >= m->height) {
		return TILE_INVALID;
	}
	if (vec2_comp(pos, m->start) == 0) {
		return TILE_START;
	}
	if (vec2_comp(pos, m->end) == 0) {
		return TILE_END;
	}
	if (set_contains(m->walls, &pos)) {
		return TILE_WALL;
	}
	return TILE_EMPTY;
}

bool maze_is_passable(const maze *m, vec2 pos) {
	if (pos.x < 0 || pos.y < 0 || pos.x >= m->width || pos.y >= m->height) {
		return false;
	}
	return !set_contains(m->walls, &pos);
}

list *maze_get_neighbors(const maze *m, vec2 pos) {
	static const vec2 deltas[] = {
		{-1, -1},
		{-1,  0},
		{-1,  1},
		{ 0, -1},
		{ 0,  1},
		{ 1, -1},
		{ 1,  0},
		{ 1,  1}
	};

	list *l = list_create(sizeof(vec2));
	vec2 v;
	int i;

	for (i = 0; i < N_NEIGHBORS; i++) {
		v = pos;
		vec2_add(&v, deltas[i]);
		if (maze_is_passable(m, v)) {
			list_push_back(l, &v);	
		}
	}
	return l;
}


char _chr_read_non_whitespace(FILE *instream) {
	char c;
	do {
		c = fgetc(instream);
	} while (isspace(c));
	return c;
}
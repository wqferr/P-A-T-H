#include "core/maze.h"

#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>

#include "struct/set.h"

struct maze {
	set *walls;
	size_t width;
	size_t height;

	vec2 start;
	vec2 end;
};


char _chr_read_non_whitespace(FILE *instream);
int32_t _vec2_hash(const void *ref);
int _vec2_comp(const void *ref1, const void *ref2);

maze *maze_read(FILE *instream) {
	maze *m = malloc(sizeof(*m));
	size_t w, h;
	vec2 pos;
	char c;

	fscanf(instream, "%zu %zu", &h, &w);
	m->walls = set_create(sizeof(vec2), &_vec2_hash, &_vec2_comp);

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


char _chr_read_non_whitespace(FILE *instream) {
	char c;
	do {
		c = fgetc(instream);
	} while (isspace(c));
	return c;
}

int32_t _vec2_hash(const void *ref) {
	vec2 v = *((const vec2 *) ref);
	return 31*v.x + v.y;
}

int _vec2_comp(const void *ref1, const void *ref2) {
	vec2 u = *((const vec2 *) ref1);
	vec2 v = *((const vec2 *) ref2);
	if (u.x == v.x && u.y == v.y) {
		return 0;
	}
	return 1;
}
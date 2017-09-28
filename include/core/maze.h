#ifndef MAZE_H
#define MAZE_H 1

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#include "struct/vec2.h"

typedef enum tile {
	TILE_INVALID = 'x',
	TILE_EMPTY = '*',
	TILE_WALL = '-',
	TILE_START = '#',
	TILE_END = '$'
} tile;

typedef struct maze maze;

maze *maze_read(FILE *instream);
void maze_destroy(maze *m);

void maze_get_dimensions(const maze *m, size_t *w, size_t *h);
vec2 maze_get_start(const maze *m);
vec2 maze_get_end(const maze *m);
tile maze_get_tile(const maze *m, vec2 pos);
bool maze_is_passable(const maze *m, vec2 pos);

#endif
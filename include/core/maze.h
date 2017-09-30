/*
 * William Quelho Ferreira
 */

#ifndef MAZE_H
#define MAZE_H 1

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#include "struct/vec2.h"
#include "struct/list.h"

typedef enum tile {
	/* Invalid tile. */
	TILE_INVALID = 'x',
	/* Empty tile. Can be used for movement. */
	TILE_EMPTY = '*',
	/* Wall tile. Blocks movement. */
	TILE_WALL = '-',
	/* Start tile. */
	TILE_START = '#',
	/* End tile. */
	TILE_END = '$'
} tile;

typedef struct maze maze;

/*
 * Creates a maze based on the values given by instream.
 * The first line must be two space separated positive integers m and n.
 * The next n lines must be m characters long each (excluding end of line
 * characters) and must be entirely made of TILE_EMPTY, TILE_WALL, TILE_START
 * and TILE_END.
 * Behaviour for inputs with more than one TILE_START or more than one TILE_END
 * is undefined.
 */
maze *maze_read(FILE *instream);
/*
 * Frees memory allocated by maze_read.
 */
void maze_destroy(maze *m);

/*
 * Stores the maze's width and height in w and h, respectively.
 */
void maze_get_dimensions(const maze *m, size_t *w, size_t *h);
/*
 * Returns the coordinates of the start of the maze.
 */
vec2 maze_get_start(const maze *m);
/*
 * Returns the coordinates of the end of the maze.
 */
vec2 maze_get_end(const maze *m);
/*
 * Returns the tile at the given position.
 */
tile maze_get_tile(const maze *m, vec2 pos);
/*
 * Returns whether it is possible to move to pos.
 */
bool maze_is_passable(const maze *m, vec2 pos);

/*
 * Returns a list of tiles accessible in one movement from pos.
 * Only includes tiles which can be used for movement.
 */
list *maze_get_neighbors(const maze *m, vec2 pos);

#endif
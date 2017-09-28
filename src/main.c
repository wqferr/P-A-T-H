#include <stdio.h>

#include "core/maze.h"


void maze_print(const maze *m) {
	vec2 pos;
	size_t w, h;

	maze_get_dimensions(m, &w, &h);
	for (pos.y = 0; pos.y < h; pos.y++) {
		for (pos.x = 0; pos.x < w; pos.x++) {
			printf("%c", (char) maze_get_tile(m, pos));
		}
		printf("\n");
	}
}

int main(int argc, char const *argv[]) {
	maze *m = maze_read(stdin);
	maze_print(m);
	maze_destroy(m);
	return 0;
}
#ifndef _DUNGEON_H_
#define _DUNGEON_H_

#include "sprite_renderer.h"

struct Dungeon {
	unsigned int door_texture, floor_texture, wall_texture;
	char **map;
	unsigned short size;

	Dungeon(unsigned short);
	~Dungeon();
};

void dungeon_print(Dungeon *);
void dungeon_render(Dungeon *, SpriteRenderer *);

#endif

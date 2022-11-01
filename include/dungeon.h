#ifndef _DUNGEON_H_
#define _DUNGEON_H_

#include "types.h"
#include "sprite_renderer.h"

struct Dungeon {
	uint door_texture, floor_texture, wall_texture;
	char **map;
	u16 size;

	Dungeon(u16);
	~Dungeon();
};

void dungeon_print(Dungeon *);
void dungeon_render(Dungeon *, SpriteRenderer *);

#endif

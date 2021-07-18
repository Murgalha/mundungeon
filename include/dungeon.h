#ifndef _DUNGEON_H_
#define _DUNGEON_H_

#include "types.h"
#include "sprite_renderer.h"

typedef struct {
	uint door_texture, floor_texture, wall_texture;
	char **map;
	u16 size;
} Dungeon;

void dungeon_print(Dungeon *);
Dungeon *dungeon_new(u16);
void dungeon_render(Dungeon *, SpriteRenderer *);
void dungeon_delete(Dungeon *);

#endif

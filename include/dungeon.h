#ifndef _DUNGEON_H_
#define _DUNGEON_H_

#include <map>
#include "sprite_renderer.h"
#include "dungeon_tile.h"

struct Dungeon {
	std::map<DungeonTile, unsigned int> sprites;
	DungeonTile **map;
	unsigned short size;

	Dungeon(unsigned short);
	~Dungeon();
};

void dungeon_print(Dungeon *);
void dungeon_render(Dungeon *, SpriteRenderer *);

#endif

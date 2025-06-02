#ifndef _DUNGEON_MAP_H_
#define _DUNGEON_MAP_H_

#include "dungeon/dungeon_tile.h"
#include "dungeon/dungeon_element.h"

struct DungeonMap {
	DungeonTile **tiles;
	unsigned short tiles_size;
	DungeonElement *elements;
	int n_elements;
};

#endif

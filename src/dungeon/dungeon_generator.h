#ifndef _DUNGEON_GENERATOR_H_
#define _DUNGEON_GENERATOR_H_

#include "utils.h"
#include "types.h"
#include "dungeon/dungeon_tile.h"

struct DungeonGenerator {
	DungeonTile **map;
	unsigned short size;
	V2 *walls;
	unsigned short nwalls;
	V2 *corridors;
	unsigned short ncorridors;

	static DungeonTile **new_map(unsigned short);
};

#endif

#ifndef _DUNGEON_GENERATOR_H_
#define _DUNGEON_GENERATOR_H_

#include "utils.h"
#include "types.h"
#include "dungeon_tile.h"

struct DungeonGenerator {
	DungeonTile **map;
	unsigned short size;
	V2 *walls;
	unsigned short nwalls;
	V2 *corridors;
	unsigned short ncorridors;

	static DungeonTile **new_map(unsigned short);
};

void dungeon_generator_print(DungeonGenerator *);

#endif

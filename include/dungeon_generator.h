#ifndef _DUNGEON_GENERATOR_H_
#define _DUNGEON_GENERATOR_H_

#include "utils.h"
#include "types.h"

/*
#define FLOOR 32 // <Space>
#define WALL 35  // #
#define EMPTY 46 // .
#define DOOR 68  // D
#define CORRIDOR 67  // C
*/

struct DungeonGenerator {
	char **map;
	unsigned short size;
	V2 *walls;
	unsigned short nwalls;
	V2 *corridors;
	unsigned short ncorridors;

	static char **new_map(unsigned short);
};

void dungeon_generator_print(DungeonGenerator *);

#endif

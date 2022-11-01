#ifndef _DUNGEON_GENERATOR_H_
#define _DUNGEON_GENERATOR_H_

#include "types.h"
#include "utils.h"

/*
#define FLOOR 32 // <Space>
#define WALL 35  // #
#define EMPTY 46 // .
#define DOOR 68  // D
#define CORRIDOR 67  // C
*/

struct DungeonGenerator {
	char **map;
	u16 size;
	V2 *walls;
	u16 nwalls;
	V2 *corridors;
	u16 ncorridors;

	static char **new_map(u16);
};

void dungeon_generator_print(DungeonGenerator *);

#endif

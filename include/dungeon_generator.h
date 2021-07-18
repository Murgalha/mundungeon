#ifndef _DUNGEON_GENERATOR_H_
#define _DUNGEON_GENERATOR_H_

#include "utils.h"

/*
#define FLOOR 32 // <Space>
#define WALL 35  // #
#define EMPTY 46 // .
#define DOOR 68  // D
#define CORRIDOR 67  // C
*/

#define FLOOR 1
#define CORRIDOR 1
#define WALL 0
#define EMPTY 4
#define DOOR 2

typedef enum {
	UP,
	DOWN,
	LEFT,
	RIGHT
} Direction;

typedef struct {
	char **map;
	u16 size;
	V2 *walls;
	u16 nwalls;
	V2 *corridors;
	u16 ncorridors;
} DungeonGenerator;

void dungeon_generator_print(DungeonGenerator *);
char **dungeon_generator_new_map(u16);

#endif

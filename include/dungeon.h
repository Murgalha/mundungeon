#ifndef _DUNGEON_H_
#define _DUNGEON_H_

#include "types.h"

enum {
	FLOOR = 32, // <Space>
	WALL = 35,  // #
	EMPTY = 46, // .
	DOOR = 68,  // D
	CORRIDOR = 67,  // C
};

typedef enum {
	UP,
	DOWN,
	LEFT,
	RIGHT
} Direction;

typedef struct {
	union {
		i16 x, col;
	};
	union {
		i16 y, row;
	};
} V2;

typedef struct {
	char **map;
	u16 size;
	V2 *walls;
	u16 nwalls;
	V2 *corridors;
	u16 ncorridors;
} Dungeon;

void dungeon_print(Dungeon *);
Dungeon *dungeon_new(u16);

#endif

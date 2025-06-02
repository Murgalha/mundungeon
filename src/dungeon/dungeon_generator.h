#ifndef _DUNGEON_GENERATOR_H_
#define _DUNGEON_GENERATOR_H_

#include "utils.h"
#include "types.h"
#include "dungeon/dungeon_map.h"
#include "shapes/point.h"

typedef struct BranchablePoint {
	Point point;
	Direction direction;
} BranchablePoint;

struct DungeonGenerator {
	DungeonMap *map;

	BranchablePoint *branchable_walls;
	size_t nwalls;
	BranchablePoint *branchable_corridors;
	size_t ncorridors;

	unsigned short width();
	unsigned short height();
	static DungeonMap *new_map(unsigned short);
};

#endif

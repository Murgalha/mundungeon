#ifndef _DUNGEON_ELEMENT_H_
#define _DUNGEON_ELEMENT_H_

#include "shapes/point.h"
#include "dungeon/dungeon_element_type.h"

typedef struct DungeonElement {
	DungeonElementType type;
	Point position;
	DungeonElement(DungeonElementType type, Point position);
} DungeonElement;

#endif

#ifndef _DUNGEON_TILE_H_
#define _DUNGEON_TILE_H_

enum class DungeonTile {
	Unknown,
	Floor,
	Wall,
	ClosedTrapdoor,
	OpenTrapdoor,
	Empty,
};

char to_char(DungeonTile);
DungeonTile to_dungeon_tile(char);

#endif

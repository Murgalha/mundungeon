#include "dungeon/dungeon_tile.h"

char to_char(DungeonTile tile) {
	switch(tile) {
	case DungeonTile::Unknown:
		return '=';
		break;
	case DungeonTile::Floor:
		return ' ';
		break;
	case DungeonTile::Door:
		return 'D';
		break;
	case DungeonTile::Corridor:
		return 'C';
		break;
	case DungeonTile::Wall:
		return '#';
		break;
	case DungeonTile::Empty:
		return '.';
		break;
	default:
		return '=';
		break;
	}
}

DungeonTile to_dungeon_tile(char c) {
	switch(c) {
	case '=':
		return DungeonTile::Unknown;
		break;
	case ' ':
		return DungeonTile::Floor;
		break;
	case 'D':
		return DungeonTile::Door;
		break;
	case 'C':
		return DungeonTile::Corridor;
		break;
	case '#':
		return DungeonTile::Wall;
		break;
	case '.':
		return DungeonTile::Empty;
		break;
	default:
		return DungeonTile::Unknown;
		break;
	}
}

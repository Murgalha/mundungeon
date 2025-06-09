#include "dungeon/dungeon_tile.h"

char to_char(DungeonTile tile) {
	switch(tile) {
	case DungeonTile::Unknown:
		return '=';
		break;
	case DungeonTile::Floor:
		return ' ';
		break;
	case DungeonTile::Wall:
		return '#';
		break;
	case DungeonTile::ClosedTrapdoor:
		return 'X';
		break;
	case DungeonTile::OpenTrapdoor:
		return 'O';
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
	case '#':
		return DungeonTile::Wall;
		break;
	case 'X':
		return DungeonTile::ClosedTrapdoor;
		break;
	case 'O':
		return DungeonTile::OpenTrapdoor;
		break;
	case '.':
		return DungeonTile::Empty;
		break;
	default:
		return DungeonTile::Unknown;
		break;
	}
}

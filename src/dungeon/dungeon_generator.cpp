#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "dungeon/dungeon_generator.h"
#include "dungeon/dungeon_tile.h"
#include "shapes/rect.h"
#include "random.h"

#define MAX_TRIES 20

#define unsigned short ushort

Rect rooms[3] = {
	{ .start_point = { .x = 0, .y = 0}, .width = 7, .height = 5 },
	{ .start_point = { .x = 0, .y = 0}, .width = 9, .height = 7 },
	{ .start_point = { .x = 0, .y = 0}, .width = 11, .height = 9 }
};

Rect corridors[2] = {
	{ .start_point = { .x = 0, .y = 0}, .width = 3, .height = 3 },
	{ .start_point = { .x = 0, .y = 0}, .width = 3, .height = 5 },
};

Direction directions[4] = { UP, DOWN, LEFT, RIGHT };
char direction_strings[4][10] = { "up", "down", "left", "right" };

ushort DungeonGenerator::width() {
	return map->tiles_size;
}

ushort DungeonGenerator::height() {
	return map->tiles_size;
}

void shuffle_directions() {
	size_t array_len = sizeof(directions)/sizeof(directions[0]);

	for (size_t x = 0; x < array_len / 2; x++) {
		int i1 = rand() % array_len;
		int i2 = rand() % array_len;

		Direction tmp = directions[i1];
		directions[i1] = directions[i2];
		directions[i2] = tmp;
	}
}

void dg_fill_rect(DungeonGenerator *dungeon, Point start_point, ushort width, ushort height, DungeonTile tile) {
	for (ushort y = start_point.y; y < start_point.y + height; y++) {
		for (ushort x = start_point.x; x < start_point.x + width; x++) {
			dungeon->map->tiles[y][x] = tile;
		}
	}
}

void dg_make_room(DungeonGenerator *dungeon, Rect room) {
	dg_fill_rect(dungeon, room.start_point, room.width, room.height, DungeonTile::Wall);

	Point p;
	p.x = room.start_point.x + 1;
	p.y = room.start_point.y + 1;
	dg_fill_rect(dungeon, p, room.width - 2, room.height - 2, DungeonTile::Floor);
}

bool dg_try_make_room(DungeonGenerator *dungeon, Rect room) {
	if (room.start_point.x < 0 || room.start_point.x >= dungeon->width())
		return false;

	if (room.start_point.y < 0 || room.start_point.y >= dungeon->height())
		return false;

	if (room.start_point.x + room.width >= dungeon->width())
		return false;

	if (room.start_point.y + room.height >= dungeon->height())
		return false;

	for (int y = room.start_point.y; y < room.start_point.y + room.height; y++) {
		for (int x = room.start_point.x; x < room.start_point.x + room.width; x++) {
			if (dungeon->map->tiles[y][x] != DungeonTile::Empty)
				return false;
		}
	}

	dg_make_room(dungeon, room);
	return true;
}

bool dg_make_random_room(DungeonGenerator *dungeon) {
	size_t n_rooms = sizeof(rooms)/sizeof(rooms[0]);
	bool room_created = false;

	Rect r = rooms[rand() % n_rooms];
	Rect room = r;

	for (int i = 0; i < MAX_TRIES; i++) {
		size_t corridor_idx = rand() % dungeon->ncorridors;
		Point corridor = dungeon->branchable_corridors[corridor_idx].point;
		Direction direction = dungeon->branchable_corridors[corridor_idx].direction;

		switch (direction) {
		case UP:
			room.start_point.x = corridor.x - (room.width / 2);
			room.start_point.y = corridor.y - room.height;
			break;
		case DOWN:
			room.start_point.x = corridor.x - (room.width / 2);
			room.start_point.y = corridor.y + 1;
			break;
		case LEFT:
			room.start_point.x = corridor.x - room.width;
			room.start_point.y = corridor.y - (room.height / 2);
			break;
		case RIGHT:
			room.start_point.x = corridor.x + 1;
			room.start_point.y = corridor.y - (room.height / 2);
			break;
		}

		room_created = dg_try_make_room(dungeon, room);

		if (room_created) {
			Point available_wall, p;

			// Connecting the wall to the corridor
			if (direction == UP) {
				p = corridor;
				p.y -= 1;
				dungeon->map->tiles[p.y][p.x] = DungeonTile::Floor;
			}
			else if (direction == DOWN) {
				p = corridor;
				p.y += 1;
				dungeon->map->tiles[p.y][p.x] = DungeonTile::Floor;
			}
			else if (direction == LEFT) {
				p = corridor;
				p.x -= 1;
				dungeon->map->tiles[p.y][p.x] = DungeonTile::Floor;
			}
			else {
				p = corridor;
				p.x += 1;
				dungeon->map->tiles[p.y][p.x] = DungeonTile::Floor;
			}

			// Adding the 3 remaining walls of a room as available to branch
			// The 4th one was connected to the corridor
			if (direction != UP) {
				available_wall = room.start_point;
				available_wall.y += (room.height -1);
				available_wall.x += (room.width / 2);
				dungeon->branchable_walls[dungeon->nwalls].point = available_wall;
				dungeon->branchable_walls[dungeon->nwalls++].direction = DOWN;
			}
			if (direction != DOWN) {
				available_wall = room.start_point;
				available_wall.x += (room.width / 2);
				dungeon->branchable_walls[dungeon->nwalls].point = available_wall;
				dungeon->branchable_walls[dungeon->nwalls++].direction = UP;
			}
			if (direction != LEFT) {
				available_wall = room.start_point;
				available_wall.x += (room.width - 1);
				available_wall.y += (room.height / 2);
				dungeon->branchable_walls[dungeon->nwalls].point = available_wall;
				dungeon->branchable_walls[dungeon->nwalls++].direction = RIGHT;
			}
			if (direction != RIGHT) {
				available_wall = room.start_point;
				available_wall.y += (room.height / 2);
				dungeon->branchable_walls[dungeon->nwalls].point = available_wall;
				dungeon->branchable_walls[dungeon->nwalls++].direction = LEFT;
			}

			dungeon->branchable_corridors[corridor_idx] = dungeon->branchable_corridors[--(dungeon->ncorridors)];
			dungeon->map->tiles[corridor.y][corridor.x] = DungeonTile::Floor;
			return true;
		}
	}
	return false;
}

bool dg_make_random_corridor(DungeonGenerator *dungeon) {
    int tmp;
	size_t n_corridors = sizeof(corridors)/sizeof(corridors[0]);
	bool room_created = false;

	Rect r = corridors[rand() % n_corridors];
	Rect room = r;

	for (int i = 0; i < MAX_TRIES; i++) {
		size_t wall_idx = rand() % dungeon->nwalls;
		Point wall = dungeon->branchable_walls[wall_idx].point;
		Direction direction = dungeon->branchable_walls[wall_idx].direction;

        // corridors are defined as vertical on the global var,
        //so we rotate them 90 degrees trying to place them on LEFT/RIGHT
		switch (direction) {
		case UP:
			room.start_point.x = wall.x - (room.width / 2);
			room.start_point.y = wall.y - room.height;
			break;
		case DOWN:
			room.start_point.x = wall.x - (room.width / 2);
			room.start_point.y = wall.y + 1;
			break;
		case LEFT:
			tmp = room.width;
			room.width = room.height;
			room.height = tmp;

			room.start_point.x = wall.x - room.width;
			room.start_point.y = wall.y - (room.height / 2);
			break;
		case RIGHT:
			tmp = room.width;
			room.width = room.height;
			room.height = tmp;

			room.start_point.x = wall.x + 1;
			room.start_point.y = wall.y - (room.height / 2);
			break;
		}

		room_created = dg_try_make_room(dungeon, room);

		if (room_created) {
			Point available_corridor, p;

			// Here we add the new corridor walls as available to branch and connect to the previous room
			// eliminating adjacent walls
			if (direction == UP) {
				available_corridor = room.start_point;
				available_corridor.x += (room.width / 2);
				dungeon->branchable_corridors[dungeon->ncorridors].point = available_corridor;
				dungeon->branchable_corridors[dungeon->ncorridors++].direction = UP;

				p = wall;
				p.y -= 1;
				dungeon->map->tiles[p.y][p.x] = DungeonTile::Floor;
			}
			else if (direction == DOWN) {
				available_corridor = room.start_point;
				available_corridor.x += (room.width / 2);
				available_corridor.y += (room.height - 1);
				dungeon->branchable_corridors[dungeon->ncorridors].point = available_corridor;
				dungeon->branchable_corridors[dungeon->ncorridors++].direction = DOWN;

				p = wall;
				p.y += 1;
				dungeon->map->tiles[p.y][p.x] = DungeonTile::Floor;
			}
			else if (direction == LEFT) {
				available_corridor = room.start_point;
				available_corridor.y += (room.height / 2);
				dungeon->branchable_corridors[dungeon->ncorridors].point = available_corridor;
				dungeon->branchable_corridors[dungeon->ncorridors++].direction = LEFT;

				p = wall;
				p.x -= 1;
				dungeon->map->tiles[p.y][p.x] = DungeonTile::Floor;
			}
			else {
				available_corridor = room.start_point;
				available_corridor.x += (room.width - 1);
				available_corridor.y += (room.height / 2);
				dungeon->branchable_corridors[dungeon->ncorridors].point = available_corridor;
				dungeon->branchable_corridors[dungeon->ncorridors++].direction = RIGHT;

				p = wall;
				p.x += 1;
				dungeon->map->tiles[p.y][p.x] = DungeonTile::Floor;
			}

			dungeon->branchable_walls[wall_idx] = dungeon->branchable_walls[--(dungeon->nwalls)];
			dungeon->map->tiles[wall.y][wall.x] = DungeonTile::Floor;
			return true;
		}
	}
	return false;
}

void dg_make_initial_room(DungeonGenerator *dungeon) {
    // Creating initial room in the middle of the map
    Rect room;
    room.width = 9;
    room.height = 7;
    room.start_point.x = (dungeon->width() / 2) - (room.width / 2);
    room.start_point.y = (dungeon->height() / 2) - (room.height / 2);
    dg_make_room(dungeon, room);

    Point available_wall = room.start_point;
    available_wall.x += (room.width / 2);
    dungeon->branchable_walls[dungeon->nwalls].point = available_wall;
    dungeon->branchable_walls[dungeon->nwalls++].direction = UP;

	available_wall = room.start_point;
    available_wall.x += (room.width / 2);
    available_wall.y += (room.height - 1);
    dungeon->branchable_walls[dungeon->nwalls].point = available_wall;
    dungeon->branchable_walls[dungeon->nwalls++].direction = DOWN;

    available_wall = room.start_point;
    available_wall.y += (room.height / 2);
    dungeon->branchable_walls[dungeon->nwalls].point = available_wall;
    dungeon->branchable_walls[dungeon->nwalls++].direction = LEFT;

	available_wall = room.start_point;
    available_wall.x += (room.width - 1);
    available_wall.y += (room.height / 2);
    dungeon->branchable_walls[dungeon->nwalls].point = available_wall;
    dungeon->branchable_walls[dungeon->nwalls++].direction = RIGHT;

	Point hero_start;
	hero_start.x = (dungeon->width() / 2);
	hero_start.y = (dungeon->height() / 2);
	DungeonElement hero = DungeonElement(DungeonElementType::Hero, hero_start);

	dungeon->map->elements[dungeon->map->n_elements++] = hero;
}

DungeonMap *DungeonGenerator::new_map(ushort size) {
    DungeonGenerator *dungeon = (DungeonGenerator *)malloc(sizeof(DungeonGenerator));

    // Allocating the dungeon map
    dungeon->map = (DungeonMap *)malloc(sizeof(DungeonMap));

    dungeon->map->tiles_size = size;
    dungeon->map->tiles = (DungeonTile **)malloc(sizeof(DungeonTile *) * dungeon->height());
    for(int i = 0; i < dungeon->height(); i++) {
        (dungeon->map->tiles)[i] = (DungeonTile *)malloc(sizeof(DungeonTile) * dungeon->width());
    }

    dungeon->map->elements = (DungeonElement *)malloc(sizeof(DungeonElement) * dungeon->width() * dungeon->height());
    dungeon->map->n_elements = 0;


	// TODO: Make them dynamically resizable
    dungeon->branchable_walls = (BranchablePoint *)malloc(sizeof(BranchablePoint) * dungeon->width() * dungeon->height());
    dungeon->nwalls = 0;
    dungeon->branchable_corridors = (BranchablePoint *)malloc(sizeof(BranchablePoint) * dungeon->width() * dungeon->height());
    dungeon->ncorridors = 0;

    // Filling map with EMPTY
    Point d_begin;
    d_begin.x = d_begin.y = 0;
    dg_fill_rect(dungeon, d_begin, dungeon->width(), dungeon->height(), DungeonTile::Empty);

    dg_make_initial_room(dungeon);

    int k = 0;
    while(k != 3) {
        dg_make_random_corridor(dungeon);
        if(!dg_make_random_room(dungeon)) {
            k++;
        }
        else {
            k = 0;
        }
    }

    for (ushort y = 0; y < dungeon->height(); y++) {
        for (ushort x = 0; x < dungeon->width(); x++) {
            printf("%c ", to_char(dungeon->map->tiles[y][x]));
        }
        printf("\n");
    }

    return dungeon->map;
}

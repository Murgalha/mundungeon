#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_TRIES 20

//#define FLOOR 1
//#define CORRIDOR 1
//#define WALL 0
//#define EMPTY 4
//#define DOOR 2
#define FLOOR 32 // <Space>
#define WALL 35  // #
#define EMPTY 46 // .
#define DOOR 68  // D
#define CORRIDOR 67  // C

// Signed
#define i8 int8_t
#define i16 int16_t
#define i32 int32_t
#define i64 int64_t

// Unsigned
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t
#define uint unsigned int

// Floating point
#define f32 float

typedef struct {
	union {
		i16 x, col;
	};
	union {
		i16 y, row;
	};
} V2;

typedef struct {
	union {
		f32 x, col;
	};
	union {
		f32 y, row;
	};
} V2f;

typedef enum {
	UP = 0,
	DOWN,
	LEFT,
	RIGHT
} DIRECTION;

typedef struct {
	char **map;
	u16 size;
	V2 *walls;
	u16 nwalls;
	V2 *corridors;
	u16 ncorridors;
} DungeonGenerator;

static u64 x, y, z;

void random_init(u32 seed) {
	srand(seed);
	x = rand();
	y = rand();
	z = rand();
}

u64 random_u64() {
	u64 t;
	x ^= x << 16;
	x ^= x >> 5;
	x ^= x << 1;

	t = x;
	x = y;
	y = z;
	z = t ^ x ^ y;

	return z;
}

u64 random_rangei(u64 begin, u64 end) {
	return (random_u64() % (end - begin)) + begin;
}

// ======================
// BEGIN TILE FUNCTIONS
// ======================
bool tile_is_corner(DungeonGenerator *dungeon, V2 tile) {
	// up * left + up * right + down * left + down * right
	// (up + down) * (left + right)
	if((tile.x - 1) < 0 || (tile.x + 1) >= dungeon->size ||
	   (tile.y - 1) < 0 || (tile.y + 1) >= dungeon->size) {
		return false;
	}
	if((dungeon->map[tile.y - 1][tile.x] == EMPTY ||
		dungeon->map[tile.y + 1][tile.x] == EMPTY) &&
	   (dungeon->map[tile.y][tile.x - 1] == EMPTY ||
		dungeon->map[tile.y][tile.x + 1] == EMPTY)) {
		return true;
	}
	return false;
}

V2 get_random_wall(DungeonGenerator *dungeon) {
	V2 wall;

	for(int i = 0; i < MAX_TRIES; i++) {
		int r = random_rangei(0, dungeon->nwalls);
		wall = dungeon->walls[r];
		if(!tile_is_corner(dungeon, wall))
			return wall;
	}
	wall.x = wall.y = -1;
	return wall;
}

V2 get_random_corridor(DungeonGenerator *dungeon) {
	V2 corridor;

	/* This function might not need a for loop, because all corridors are
	   valid. If only end of corridors are available, this might be needed. But
	   then I could just store end of corridors on the array. */
	for(int i = 0; i < MAX_TRIES; i++) {
		int r = random_rangei(0, dungeon->ncorridors);
		corridor = dungeon->corridors[r];
		return corridor;
	}
	corridor.x = corridor.y = -1;
	return corridor;
}

V2 direction_vector(DIRECTION d) {
	V2 offset;
	offset.x = offset.y = 0;

	switch(d) {
	case UP:
		offset.y = -1;
		break;
	case DOWN:
		offset.y = 1;
		break;
	case LEFT:
		offset.x = -1;
		break;
	case RIGHT:
		offset.x = 1;
		break;
	}
	return offset;
}
// ======================
// END TILE FUNCTIONS
// ======================


void dungeon_generator_print(DungeonGenerator *dungeon) {
	for(int y = 0; y < dungeon->size; y++) {
		for(int x = 0; x < dungeon->size; x++) {
			printf("%c", dungeon->map[y][x]);
		}
		printf("| Row %d\n", y);
	}
}

void dungeon_generator_fill_rect(DungeonGenerator *dungeon, V2 begin, int width, int height, char element) {
	V2 end;
	end.x = begin.x + width;
	end.y = begin.y + height;

	for(int y = begin.y; y < end.y; y++) {
		for(int x = begin.x; x < end.x; x++) {
			dungeon->map[y][x] = element;
		}
	}
}

// =========================
// BEGIN ROOM FUNCTIONS
// =========================
void dungeon_generator_make_room_at(DungeonGenerator *dungeon, V2 begin, int width, int height) {
	V2 end, tile;
	end.x = begin.x + width;
	end.y = begin.y + height;

	// Inserting horizontal walls
	for(int x = begin.x; x < end.x; x++) {
		tile.x = x;

		tile.y = begin.y;
		dungeon->map[tile.y][tile.x] = WALL;
		dungeon->walls[dungeon->nwalls++] = tile;

		tile.y = end.y - 1;
		dungeon->map[tile.y][tile.x] = WALL;
		dungeon->walls[dungeon->nwalls++] = tile;
	}

	// Inserting vertical walls
	for(int y = begin.y; y < end.y; y++) {
		tile.y = y;

		tile.x = begin.x;
		dungeon->map[tile.y][tile.x] = WALL;
		dungeon->walls[dungeon->nwalls++] = tile;

		tile.x = end.x - 1;
		dungeon->map[tile.y][tile.x] = WALL;
		dungeon->walls[dungeon->nwalls++] = tile;
	}
	begin.x++;
	begin.y++;
	width -= 2;
	height -= 2;

	dungeon_generator_fill_rect(dungeon, begin, width, height, FLOOR);
}

bool dungeon_generator_has_room_space(DungeonGenerator *dungeon, V2 door, int width, int height, DIRECTION d) {
	V2 begin = door;

	switch(d) {
	case UP:
		begin.y--;
		for(int y = begin.y; y > begin.y - height; y--) {
			for(int x = begin.x - width/2; x < begin.x + width/2; x++) {
				if((x < 0 || x >= dungeon->size) ||
				   (y < 0 || y >= dungeon->size)) {
					return false;
				}
				if(dungeon->map[y][x] != EMPTY && dungeon->map[y][x] != WALL) {
					return false;
				}
			}
		}
		return true;
		break;
	case DOWN:
		begin.y++;
		for(int y = begin.y; y < begin.y + height; y++) {
			for(int x = begin.x - width/2; x < begin.x + width/2; x++) {
				if((x < 0 || x >= dungeon->size) ||
				   (y < 0 || y >= dungeon->size)) {
					return false;
				}
				if(dungeon->map[y][x] != EMPTY && dungeon->map[y][x] != WALL) {
					return false;
				}
			}
		}
		return true;
		break;
	case LEFT:
		begin.x--;
		for(int y = begin.y-height/2; y < begin.y + height/2; y++) {
			for(int x = begin.x; x > begin.x - width/2; x--) {
				if((x < 0 || x >= dungeon->size) ||
				   (y < 0 || y >= dungeon->size)) {
					return false;
				}
				if(dungeon->map[y][x] != EMPTY && dungeon->map[y][x] != WALL) {
					return false;
				}
			}
		}
		return true;
		break;
	case RIGHT:
		begin.x++;
		for(int y = begin.y-height/2; y < begin.y + height/2; y++) {
			for(int x = begin.x; x < begin.x + width/2; x++) {
				if((x < 0 || x >= dungeon->size) ||
				   (y < 0 || y >= dungeon->size)) {
					return false;
				}
				if(dungeon->map[y][x] != EMPTY && dungeon->map[y][x] != WALL) {
					return false;
				}
			}
		}
		return true;
		break;
	default:
		return false;
	}
}

bool dungeon_generator_make_random_room(DungeonGenerator *dungeon) {
	V2 corridor;
	int counter = 0;
	do {
		corridor = get_random_corridor(dungeon);
		counter++;
		if(counter == 100) {
			break;
		}
	} while ((corridor.x == -1 && corridor.y == -1));

	if(counter == 100 || corridor.x == -1) {
		printf("Could not get random wall. Aborting...\n");
		return false;
	}

	int size = random_rangei(5, 9);

	// TODO: Make shuffled array of directions so the
	// algorithm stays "more random"
	for(DIRECTION d = 0; d < 4; d++) {
		if(dungeon_generator_has_room_space(dungeon, corridor, size, size, d)) {
			V2 begin, door;
			int width, height;
			width = height = size;

			switch(d) {
			case UP:
				door.x = corridor.x;
				door.y = corridor.y - 1;
				begin.x = corridor.x - width/2;
				begin.y = corridor.y - height;
				break;
			case DOWN:
				door.x = corridor.x;
				door.y = corridor.y + 1;
				begin.x = corridor.x - width/2;
				begin.y = corridor.y + 1;
				break;
			case LEFT:
				door.x = corridor.x - 1;
				door.y = corridor.y;
				begin.x = corridor.x - width;
				begin.y = corridor.y - height/2;
				break;
			case RIGHT:
				door.x = corridor.x + 1;
				door.y = corridor.y;
				begin.x = corridor.x + 1;
				begin.y = corridor.y - height/2;
				break;
			}
			dungeon_generator_make_room_at(dungeon, begin, size, size);
			dungeon->map[door.y][door.x] = DOOR;
			return true;
		}
	}
	return false;
}
// =========================
// END ROOM FUNCTIONS
// =========================

// =========================
// BEGIN CORRIDOR FUNCTIONS
// =========================
void dungeon_generator_make_corridor_at(DungeonGenerator *dungeon, V2 door, int size, DIRECTION d) {
	V2 vector = direction_vector(d);
	V2 tmp;
	int i;

	dungeon->map[door.y][door.x] = DOOR;
	for(i = 1; i < size; i++) {
		tmp.x = door.x + (vector.x * i);
		tmp.y = door.y + (vector.y * i);
		dungeon->map[tmp.y][tmp.x] = CORRIDOR;
	}
	// TODO: Add every corridor to the array
	dungeon->corridors[dungeon->ncorridors++] = tmp;
}

bool dungeon_generator_has_corridor_space(DungeonGenerator *dungeon, V2 begin, int size, DIRECTION d) {
	V2 vector = direction_vector(d);
	V2 tmp;

	for(int i = 1; i < size; i++) {
		tmp.x = begin.x + (vector.x * i);
		tmp.y = begin.y + (vector.y * i);

		if(tmp.x < 0 || tmp.x >= dungeon->size ||
		   tmp.y < 0 || tmp.y >= dungeon->size) {
			return false;
		}

		if(dungeon->map[tmp.y][tmp.x] != EMPTY) {
			return false;
		}
	}
	return true;
}

void dungeon_generator_make_random_corridor(DungeonGenerator *dungeon) {
	V2 wall;
	int counter = 0;
	do {
		wall = get_random_wall(dungeon);
		counter++;
		if(counter == 100) {
			break;
		}
	} while ((wall.x == -1 && wall.y == -1));

	if(counter == 100 || wall.x == -1) {
		printf("Could not get random wall. Aborting...\n");
		return;
	}
	int size = random_rangei(5, 9);

	// TODO: Make shuffled array of directions so the
	// algorithm stays "more random"
	for(DIRECTION d = 0; d < 4; d++) {
		if(dungeon_generator_has_corridor_space(dungeon, wall, size, d)) {
			dungeon_generator_make_corridor_at(dungeon, wall, size, d);
			return;
		}
	}
	return;
}
// =========================
// END CORRIDOR FUNCTIONS
// =========================

char **dungeon_generator_new_map(u16 size) {
	DungeonGenerator *dungeon = malloc(sizeof(DungeonGenerator));
	dungeon->size = size;

	// Allocating the dungeon map
	dungeon->map = malloc(sizeof(char *) * dungeon->size);
	for(int i = 0; i < dungeon->size; i++) {
		(dungeon->map)[i] = malloc(sizeof(char) * dungeon->size);
	}
	// TODO: Make them dynamically resizable
	dungeon->walls = malloc(sizeof(V2) * dungeon->size * dungeon->size);
	dungeon->nwalls = 0;
	dungeon->corridors = malloc(sizeof(V2) * dungeon->size * dungeon->size);
	dungeon->ncorridors = 0;

	// Filling map with EMPTY
	V2 d_begin;
	d_begin.x = d_begin.y = 0;
	dungeon_generator_fill_rect(dungeon, d_begin, size, size, EMPTY);

	// Creating initial room in the middle of the map
	d_begin.x = size/2 - 5;
	d_begin.y = size/2 - 3;
	dungeon_generator_make_room_at(dungeon, d_begin, 10, 7);

	int k = 0;
	while(k != 5) {
		dungeon_generator_make_random_corridor(dungeon);
		if(!dungeon_generator_make_random_room(dungeon)) {
			k++;
		}
		else {
			k = 0;
		}
	}
	char **map = dungeon->map;

	free(dungeon->corridors);
	free(dungeon->walls);
	free(dungeon);

	return map;
}

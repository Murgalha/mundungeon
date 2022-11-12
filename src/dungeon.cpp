#include <stdlib.h>
#include <stdio.h>
#include "dungeon.h"
#include "dungeon_generator.h"
#include "texture.h"

void dungeon_print(Dungeon *dungeon) {
	for(int y = 0; y < dungeon->size; y++) {
		for(int x = 0; x < dungeon->size; x++) {
			printf("%c", to_char(dungeon->map[y][x]));
		}
		printf("\n");
	}
}

Dungeon::Dungeon(unsigned short dungeon_size) {
	size = dungeon_size;
	map = DungeonGenerator::new_map(size);

	unsigned int door_texture = texture_new((char *)"assets/door.png", GL_RGBA, false);
	unsigned int floor_texture = texture_new((char *)"assets/floor.png", GL_RGBA, false);
	unsigned int wall_texture = texture_new((char *)"assets/wall.png", GL_RGBA, false);
	unsigned int unknown_texture = texture_new((char *)"assets/unknown.png", GL_RGBA, false);

	sprites = std::map<DungeonTile, unsigned int>();
	sprites.insert(std::pair<DungeonTile, unsigned int>(DungeonTile::Unknown, unknown_texture));
	sprites.insert(std::pair<DungeonTile, unsigned int>(DungeonTile::Floor, floor_texture));
	sprites.insert(std::pair<DungeonTile, unsigned int>(DungeonTile::Door, door_texture));
	sprites.insert(std::pair<DungeonTile, unsigned int>(DungeonTile::Corridor, floor_texture));
	sprites.insert(std::pair<DungeonTile, unsigned int>(DungeonTile::Wall, wall_texture));
	sprites.insert(std::pair<DungeonTile, unsigned int>(DungeonTile::Empty, wall_texture));
}

Dungeon::~Dungeon() {
	for(int i = 0; i < size; i++) {
		free(map[i]);
	}
	free(map);
}

void dungeon_render(Dungeon *dungeon, SpriteRenderer *renderer) {
	DungeonTile tile;
	unsigned int texture;
	glm::vec2 position;

	for(int y = 0; y < dungeon->size; y++) {
		for(int x = 0; x < dungeon->size; x++) {
			tile = dungeon->map[y][x];
			position[0] = x * renderer->sprite_width;
			position[1] = y * renderer->sprite_height;

			texture = dungeon->sprites[tile];
			renderer->draw_sprite(texture, position);
		}
	}
}

#include <stdlib.h>
#include <stdio.h>
#include <glm/vec2.hpp>
#include "hero.h"
#include "dungeon.h"
#include "dungeon_generator.h"
#include "texture.h"
#include "imgui.h"

void spawn_enemies(Dungeon *);
void dungeon_print(Dungeon *);

Dungeon::Dungeon(unsigned short dungeon_size) {
	hero = new Hero();
	size = dungeon_size;
	map = DungeonGenerator::new_map(size);

	spawn_enemies(this);

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
		free(enemies[i]);
	}
	free(map);
	free(enemies);

	delete hero;
}

void Dungeon::post_turn_cleanup() {
	if (enemy.check_death()) {
		auto old_position = enemy.position;
		auto old_x = (int)old_position.x;
		auto old_y = (int)old_position.y;
		enemies[old_y][old_x] = 0;

		enemy = Enemy(glm::vec2(13.0f, 21.0f));

		auto new_position = enemy.position;
		auto new_x = (int)new_position.x;
		auto new_y = (int)new_position.y;

		enemies[new_y][new_x] = 1;
	}
}

void dungeon_render(Dungeon *dungeon, SpriteRenderer *renderer, float delta_time) {
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

	dungeon->enemy.draw(renderer);
	hero_render(dungeon->hero, renderer, delta_time);
}

void spawn_enemies(Dungeon *dungeon) {
	glm::vec2 positions[] = {
		glm::vec2(13.0f, 21.0f),
	};

	dungeon->enemies = (unsigned char **) malloc (sizeof(unsigned char *) * dungeon->size);
	for(int i = 0; i < dungeon->size; i++) {
		dungeon->enemies[i] = (unsigned char *) calloc (dungeon->size, sizeof(unsigned char));
	}

	for (glm::vec2 position : positions) {
		dungeon->enemy = Enemy(position);
		dungeon->enemies[(int)position.y][(int)position.x] = 1;
	}
}

void dungeon_print(Dungeon *dungeon) {
	for(int y = 0; y < dungeon->size; y++) {
		for(int x = 0; x < dungeon->size; x++) {
			printf("%c", to_char(dungeon->map[y][x]));
		}
		printf("\n");
	}
}

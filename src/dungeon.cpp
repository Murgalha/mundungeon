#include <stdlib.h>
#include <stdio.h>
#include <glm/vec2.hpp>
#include "hero.h"
#include "dungeon.h"
#include "dungeon_generator.h"
#include "texture.h"
#include "imgui.h"

void spawn_enemies(Dungeon *);

Dungeon::Dungeon(unsigned short dungeon_size) {
	_create_hero();
	size = dungeon_size;
	turn_action = HeroAction::NoAction;
	map = DungeonGenerator::new_map(size);

	spawn_enemies(this);

	uint32_t door_texture = texture_new((char *)"assets/door.png", GL_RGBA, false);
	uint32_t floor_texture = texture_new((char *)"assets/floor.png", GL_RGBA, false);
	uint32_t wall_texture = texture_new((char *)"assets/wall.png", GL_RGBA, false);
	uint32_t unknown_texture = texture_new((char *)"assets/unknown.png", GL_RGBA, false);

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

void Dungeon::update(float delta_time) {
	hero->update(turn_action, *this, delta_time);

	if (turn_action != HeroAction::NoAction) {
		glm::vec2 enemy_pos = enemy.grid_position;
		enemies[(int)enemy_pos.y][(int)enemy_pos.x] = 0;
		enemy.walk(*this, *hero);

		enemy_pos = enemy.grid_position;
		enemies[(int)enemy_pos.y][(int)enemy_pos.x] = 1;
	}

	post_turn_cleanup();

	turn_action = HeroAction::NoAction;
}

void Dungeon::post_turn_cleanup() {
	if (enemy.check_death()) {
		auto old_position = enemy.grid_position;
		auto old_x = (int)old_position.x;
		auto old_y = (int)old_position.y;
		enemies[old_y][old_x] = 0;

		// TODO: Hardcoded respawn of enemies should not exist
		enemy = Enemy(enemy.texture_id, glm::vec2(13.0f, 21.0f));

		auto new_position = enemy.grid_position;
		auto new_x = (int)new_position.x;
		auto new_y = (int)new_position.y;

		enemies[new_y][new_x] = 1;
	}
}

void Dungeon::render(SpriteRenderer &renderer) {
	DungeonTile tile;
	unsigned int texture;
	glm::vec2 position;

	for(int y = 0; y < size; y++) {
		for(int x = 0; x < size; x++) {
			tile = map[y][x];
			position[0] = x * renderer.sprite_width;
			position[1] = y * renderer.sprite_height;

			texture = sprites[tile];
			renderer.draw_sprite(texture, position);
		}
	}

	enemy.render(renderer);
	hero->render(renderer);
}

bool Dungeon::can_move_to(glm::vec2 &position) {
	DungeonTile tile = map[(int)position.y][(int)position.x];

	return tile != DungeonTile::Wall &&
		tile != DungeonTile::Empty &&
		enemies[(int)position.y][(int)position.x] != 1;
}

void spawn_enemies(Dungeon *dungeon) {
	auto tex_id = texture_new((char *)"assets/enemy.png", GL_RGBA, false);
	glm::vec2 positions[] = {
		glm::vec2(13.0f, 21.0f),
	};

	dungeon->enemies = (unsigned char **) malloc (sizeof(unsigned char *) * dungeon->size);
	for(int i = 0; i < dungeon->size; i++) {
		dungeon->enemies[i] = (unsigned char *) calloc (dungeon->size, sizeof(unsigned char));
	}

	for (glm::vec2 position : positions) {
		dungeon->enemy = Enemy(tex_id, position);
		dungeon->enemies[(int)position.y][(int)position.x] = 1;
	}
}

void Dungeon::print() {
	for(int y = 0; y < size; y++) {
		for(int x = 0; x < size; x++) {
			printf("%c", to_char(map[y][x]));
		}
		printf("\n");
	}
}

void Dungeon::_create_hero() {
	auto texture = texture_new((char *)"assets/hero.png", GL_RGBA, false);
	auto starting_position = glm::vec2(25.0, 25.0);

	hero = new Hero(texture, starting_position);
}

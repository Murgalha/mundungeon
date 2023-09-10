#include <stdlib.h>
#include <stdio.h>
#include <glm/vec2.hpp>
#include "hero.h"
#include "dungeon/dungeon.h"
#include "dungeon/dungeon_generator.h"
#include "texture.h"
#include "text_rendering/text_render_options_builder.h"

Dungeon::Dungeon(uint16_t dungeon_size) {
	is_game_over = false;
	_create_hero();
	size = dungeon_size;
	turn_action = HeroAction::NoAction;
	map = DungeonGenerator::new_map(size);
	game_over = GameOverScreen();
	camera = new Camera(hero->position);

	_spawn_enemies();

	auto door_texture = Texture((char *)"assets/door.png");
	auto floor_texture = Texture((char *)"assets/floor.png");
	auto wall_texture = Texture((char *)"assets/wall.png");
	auto unknown_texture = Texture((char *)"assets/unknown.png");

	sprites = std::map<DungeonTile, Texture>();
	sprites.insert(std::pair<DungeonTile, Texture>(DungeonTile::Unknown, unknown_texture));
	sprites.insert(std::pair<DungeonTile, Texture>(DungeonTile::Floor, floor_texture));
	sprites.insert(std::pair<DungeonTile, Texture>(DungeonTile::Door, door_texture));
	sprites.insert(std::pair<DungeonTile, Texture>(DungeonTile::Corridor, floor_texture));
	sprites.insert(std::pair<DungeonTile, Texture>(DungeonTile::Wall, wall_texture));
	sprites.insert(std::pair<DungeonTile, Texture>(DungeonTile::Empty, wall_texture));
}

Dungeon::~Dungeon() {
	for(int i = 0; i < size; i++) {
		free(map[i]);
		free(enemies[i]);
	}
	free(map);
	free(enemies);

	delete camera;
	delete hero;
}

bool Dungeon::handle_input(Input input) {
	if (is_game_over) {
		return game_over.handle_input(input);
	}

	HeroAction action = HeroAction::NoAction;
	bool handled = false;

	switch(input) {
	case Input::MoveUp:
		action = HeroAction::WalkUp;
		handled = true;
		break;
	case Input::MoveDown:
		action = HeroAction::WalkDown;
		handled = true;
		break;
	case Input::MoveLeft:
		action = HeroAction::WalkLeft;
		handled = true;
		break;
	case Input::MoveRight:
		action = HeroAction::WalkRight;
		handled = true;
		break;
	default:
		break;
	}
	turn_action = action;

	return handled;
}

void Dungeon::update(float delta_time) {
	hero->update(*this, delta_time);

	glm::vec2 enemy_pos = enemy.grid_position;
	enemies[(int)enemy_pos.y][(int)enemy_pos.x] = 0;
	enemy.update(*this, delta_time);

	enemy_pos = enemy.grid_position;
	enemies[(int)enemy_pos.y][(int)enemy_pos.x] = 1;

	_post_turn_cleanup();

	turn_action = HeroAction::NoAction;

	// This should be done last
	camera->focus_on(hero->position);
}

void Dungeon::_post_turn_cleanup() {
	if (enemy.check_death()) {
		auto old_position = enemy.grid_position;
		auto old_x = (int)old_position.x;
		auto old_y = (int)old_position.y;
		enemies[old_y][old_x] = 0;

		// TODO: Hardcoded respawn of enemies should not exist
		enemy = Enemy(enemy.texture, glm::vec2(13.0f, 21.0f));

		auto new_position = enemy.grid_position;
		auto new_x = (int)new_position.x;
		auto new_y = (int)new_position.y;

		enemies[new_y][new_x] = 1;
	}

	if (hero->is_dead()) {
		is_game_over = true;
	}
}

void Dungeon::render(SpriteRenderer &renderer, TextRenderer &text_renderer) {
	DungeonTile tile;
	Texture texture;
	glm::vec2 position;

	for(int y = 0; y < size; y++) {
		for(int x = 0; x < size; x++) {
			tile = map[y][x];
			position[0] = x * renderer.sprite_width;
			position[1] = y * renderer.sprite_height;

			texture = sprites[tile];
			renderer.render(texture, position);
		}
	}

	enemy.render(renderer);
	hero->render(renderer);

	if (is_game_over) {
		game_over.render(renderer, text_renderer, camera->position);
	}
}

bool Dungeon::can_move_to(glm::vec2 &position) {
	DungeonTile tile = map[(int)position.y][(int)position.x];

	return tile != DungeonTile::Wall &&
		tile != DungeonTile::Empty &&
		enemies[(int)position.y][(int)position.x] != 1;
}

void Dungeon::_spawn_enemies() {
	auto tex_id = Texture((char *)"assets/enemy.png");
	glm::vec2 positions[] = {
		glm::vec2(13.0f, 21.0f),
	};

	enemies = (unsigned char **) malloc (sizeof(unsigned char *) * size);
	for(int i = 0; i < size; i++) {
		enemies[i] = (unsigned char *) calloc (size, sizeof(unsigned char));
	}

	for (glm::vec2 position : positions) {
		enemy = Enemy(tex_id, position);
		enemies[(int)position.y][(int)position.x] = 1;
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
	auto texture = Texture((char *)"assets/hero.png");
	auto starting_position = glm::vec2(25.0, 25.0);

	hero = new Hero(texture, starting_position);
}

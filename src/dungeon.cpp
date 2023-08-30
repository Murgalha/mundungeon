#include <stdlib.h>
#include <stdio.h>
#include <glm/vec2.hpp>
#include "hero.h"
#include "dungeon.h"
#include "dungeon_generator.h"
#include "texture.h"
#include "imgui.h"

Dungeon::Dungeon(uint16_t dungeon_size) {
	is_game_over = false;
	_create_hero();
	size = dungeon_size;
	turn_action = HeroAction::NoAction;
	map = DungeonGenerator::new_map(size);

	_spawn_enemies();

	uint32_t door_texture = texture_new((char *)"assets/door.png", GL_RGBA, false);
	uint32_t floor_texture = texture_new((char *)"assets/floor.png", GL_RGBA, false);
	uint32_t wall_texture = texture_new((char *)"assets/wall.png", GL_RGBA, false);
	uint32_t unknown_texture = texture_new((char *)"assets/unknown.png", GL_RGBA, false);
	uint32_t white_texture = texture_new((char *)"assets/white.png", GL_RGBA, false);

	sprites = std::map<DungeonTile, uint32_t>();
	sprites.insert(std::pair<DungeonTile, uint32_t>(DungeonTile::Unknown, unknown_texture));
	sprites.insert(std::pair<DungeonTile, uint32_t>(DungeonTile::Floor, floor_texture));
	sprites.insert(std::pair<DungeonTile, uint32_t>(DungeonTile::Door, door_texture));
	sprites.insert(std::pair<DungeonTile, uint32_t>(DungeonTile::Corridor, floor_texture));
	sprites.insert(std::pair<DungeonTile, uint32_t>(DungeonTile::Wall, wall_texture));
	sprites.insert(std::pair<DungeonTile, uint32_t>(DungeonTile::Empty, wall_texture));
	sprites.insert(std::pair<DungeonTile, uint32_t>(DungeonTile::White, white_texture));
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

bool Dungeon::handle_input(Input input) {
	if (is_game_over)
		return true;

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
}

void Dungeon::_post_turn_cleanup() {
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

	if (hero->is_dead()) {
		is_game_over = true;
	}
}

void Dungeon::render(SpriteRenderer &renderer, TextRenderer &text_renderer) {
	DungeonTile tile;
	unsigned int texture;
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
		_draw_gameover(renderer, text_renderer);
	}
}

bool Dungeon::can_move_to(glm::vec2 &position) {
	DungeonTile tile = map[(int)position.y][(int)position.x];

	return tile != DungeonTile::Wall &&
		tile != DungeonTile::Empty &&
		enemies[(int)position.y][(int)position.x] != 1;
}

void Dungeon::_spawn_enemies() {
	auto tex_id = texture_new((char *)"assets/enemy.png", GL_RGBA, false);
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
	auto texture = texture_new((char *)"assets/hero.png", GL_RGBA, false);
	auto starting_position = glm::vec2(25.0, 25.0);

	hero = new Hero(texture, starting_position);
}

void Dungeon::_draw_gameover(SpriteRenderer &renderer, TextRenderer &text_renderer) {
	// draw white transparent square
	auto tex = sprites[DungeonTile::White];
	auto scale = glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT);
	auto transparent_white = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);

	auto pos = glm::vec2(0.0f);
	pos.x = hero->position.x - SCREEN_WIDTH / 2 + SPRITE_WIDTH / 2;
	pos.y = hero->position.y - SCREEN_HEIGHT / 2 + SPRITE_HEIGHT / 2;


	renderer.render(tex, pos, 0.0f, transparent_white, scale);

	auto dark_red = glm::vec3(0.8f, 0.0f, 0.0f);
	// TODO: Have perspective working properly without needing to draw text after graphics
	text_renderer.draw((char *)"GAME OVER", glm::vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2), 40.0, dark_red);
}

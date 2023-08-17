#include <stdlib.h>
#include <stdio.h>
#include <glm/gtx/compatibility.hpp>
#include "hero.h"
#include "texture.h"
#include "dungeon_tile.h"
#include "sprite_renderer.h"
#include "utils.h"
#include "dungeon.h"
#include "random.h"

float get_sprite_rotation(Direction);

Hero::Hero() {
	texture_id = texture_new((char *)"assets/hero.png", GL_RGBA, false);
	grid_position = glm::vec2(25.0, 25.0);
	real_position = grid_position * SPRITE_WIDTH;
	hp = 100;
	is_moving = false;
	animation = nullptr;
}

Hero::~Hero() {}

void Hero::render(SpriteRenderer &renderer) {
	renderer.draw_sprite_with_rotation(texture_id, real_position, get_sprite_rotation(facing_direction));
}

void Hero::update(HeroAction action, Dungeon &dungeon, float delta_time) {
	if (is_moving) {
		if (animation->has_ended()) {
			is_moving = false;
			real_position = animation->target;
			grid_position = real_position / SPRITE_WIDTH;
		}
		else {
			glm::vec2 new_position = animation->get_animation_position(delta_time);
			real_position = new_position;
		}
	}
	else {
		switch (action) {
		case HeroAction::WalkRight:
			_move(dungeon, RIGHT);
			break;
		case HeroAction::WalkLeft:
			_move(dungeon, LEFT);
			break;
		case HeroAction::WalkDown:
			_move(dungeon, DOWN);
			break;
		case HeroAction::WalkUp:
			_move(dungeon, UP);
			break;
		case HeroAction::Attack:
			_attack(dungeon);
			break;
		case HeroAction::NoAction:
		default:
			break;
		}
	}
}

void Hero::_move(Dungeon &dungeon, Direction d) {
	if (!is_moving) {
		glm::vec2 new_grid_position = grid_position + dir_array[d];

		facing_direction = d;
		if(dungeon.can_move_to(new_grid_position)) {
			glm::vec2 pixel_position = new_grid_position * SPRITE_WIDTH;
			is_moving = true;

			if (animation != nullptr) delete animation;
			animation = new AnimationCalculator(real_position, pixel_position, 300.0f);

			grid_position = new_grid_position;
		}
	}
}

void Hero::_attack(Dungeon &dungeon) {
	auto offset = dir_array[facing_direction];
	auto x = (int)(grid_position[0] + offset[0]);
	auto y = (int)(grid_position[1] + offset[1]);

	auto has_enemy = dungeon.enemies[y][x];

	printf("Hero is attacking\n");
	if (has_enemy) {
		auto r = random_rangei(1, 11);

		printf("Enemy HP before: %d\n", dungeon.enemy.hp);
		dungeon.enemy.hp -= r;
		printf("Enemy HP after: %d\n", dungeon.enemy.hp);
		dungeon.enemy.check_death();
	}
}

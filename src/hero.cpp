#include <stdlib.h>
#include <stdio.h>
#include <glm/gtx/compatibility.hpp>
#include "hero.h"
#include "texture.h"
#include "dungeon/dungeon_tile.h"
#include "sprite_renderer.h"
#include "utils.h"
#include "dungeon/dungeon.h"
#include "random.h"

float get_sprite_rotation(Direction);

Hero::Hero(Texture texture, glm::vec2 grid_start_pos) : Entity(texture, grid_start_pos * SPRITE_WIDTH) {
	grid_position = grid_start_pos;
	hp = 100;
	is_moving = false;
	animation = nullptr;
}

Hero::~Hero() {}

void Hero::render(SpriteRenderer &renderer) {
	renderer.render(texture, position, get_sprite_rotation(facing_direction));
}

void Hero::update(Dungeon &dungeon, float delta_time) {
	HeroAction action = dungeon.turn_action;
	if (is_moving) {
		if (animation->has_ended()) {
			is_moving = false;
			position = animation->get_animation_position(delta_time);
			grid_position = position / SPRITE_WIDTH;
		}
		else {
			glm::vec2 new_position = animation->get_animation_position(delta_time);
			position = new_position;
		}
		position = animation->get_animation_position(delta_time);
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

bool Hero::is_dead() {
	return hp == 0;
}

void Hero::_move(Dungeon &dungeon, Direction d) {
	if (!is_moving) {
		glm::vec2 new_grid_position = grid_position + dir_array[d];

		facing_direction = d;
		if(dungeon.can_move_to(new_grid_position)) {
			glm::vec2 pixel_position = new_grid_position * SPRITE_WIDTH;
			is_moving = true;

			if (animation != nullptr) delete animation;

			auto step = AnimationStep(position, pixel_position, 500);
			animation = new AnimationCalculator(step);

			grid_position = new_grid_position;
		}
	}
}

void Hero::_attack(Dungeon &dungeon) {
	if (!is_moving) {
		auto offset = dir_array[facing_direction];
		auto x = (int)(grid_position.x + offset.x);
		auto y = (int)(grid_position.y + offset.y);
		glm::vec2 target_position = glm::vec2(x, y);

		auto has_enemy = dungeon.enemies[y][x];

		if (has_enemy) {
			auto r = random_rangei(1, 11);
			dungeon.enemy.hp -= r;
		}

		if (animation != nullptr) delete animation;

		glm::vec2 pixel_position = target_position * SPRITE_WIDTH;
		auto steps = std::vector<AnimationStep> {
			AnimationStep(position, pixel_position, 300),
			AnimationStep(pixel_position, position, 300)
		};
		animation = new MultiAnimationCalculator(steps);
		is_moving = true;
	}
}

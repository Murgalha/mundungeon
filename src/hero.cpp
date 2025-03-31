#include <stdlib.h>
#include <stdio.h>
#include "hero.h"
#include "texture.h"
#include "dungeon/dungeon_tile.h"
#include "sprite_renderer.h"
#include "utils.h"
#include "animation/multi_animation_calculator.h"
#include "dungeon/dungeon.h"
#include "random.h"
#include "renderer_repo.h"

Hero::Hero(Texture texture, glm::vec2 grid_start_pos) : Entity(texture, grid_start_pos * SPRITE_WIDTH) {
	grid_position = grid_start_pos;
	_hp = 100;
	state = CreatureState::Idle;
	animation = nullptr;
}

Hero::~Hero() {}

void Hero::render() {
	auto renderer = renderer_repo["default"];

	renderer->render(texture, position, get_sprite_rotation(facing_direction));
}

void Hero::update(Dungeon &dungeon, float delta_time) {
	HeroAction action = dungeon.turn_action;
	if (state == CreatureState::Attacking || state == CreatureState::Moving) {
		if (animation && animation->has_ended()) {
			state = CreatureState::Idle;
			dungeon.set_enemy_turn();

			position = animation->get_animation_position(delta_time);
			grid_position = position / SPRITE_WIDTH;
		}
		else {
			glm::vec2 new_position = animation->get_animation_position(delta_time);
			position = new_position;
		}
		position = animation->get_animation_position(delta_time);
	}
	else if (state == CreatureState::Idle && dungeon.can_player_act()) {
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
	return _hp == 0;
}

void Hero::take_damage(int32_t value) {
	auto tmp = _hp - value;

	if (tmp <= 0) {
		_hp = 0;
	}
	else {
		_hp = tmp;
	}
}

int32_t Hero::hp() {
	return _hp;
}

void Hero::_move(Dungeon &dungeon, Direction d) {
	glm::vec2 new_grid_position = grid_position + dir_array[d];

	facing_direction = d;
	if(dungeon.can_move_to(new_grid_position)) {
		glm::vec2 pixel_position = new_grid_position * SPRITE_WIDTH;

		if (animation != nullptr) delete animation;

		auto step = AnimationStep(position, pixel_position, 200);
		animation = new AnimationCalculator(step);

		grid_position = new_grid_position;
		state = CreatureState::Moving;
	}
}

void Hero::_attack(Dungeon &dungeon) {
	auto offset = dir_array[facing_direction];
	auto x = (int)(grid_position.x + offset.x);
	auto y = (int)(grid_position.y + offset.y);
	glm::vec2 target_position = glm::vec2(x, y);

	auto has_enemy = dungeon.enemies[y][x];

	if (has_enemy) {
		auto r = random_rangei(1, 11);
		dungeon.enemy.take_damage(r);
	}

	if (animation != nullptr) delete animation;

	glm::vec2 pixel_position = target_position * SPRITE_WIDTH;
	auto steps = std::vector<AnimationStep> {
		AnimationStep(position, pixel_position, 150),
		AnimationStep(pixel_position, position, 150)
	};
	animation = new MultiAnimationCalculator(steps);

	state = CreatureState::Attacking;
}

#include <glm/vec2.hpp>
#include "sprite_renderer.h"
#include "enemy.h"
#include "texture.h"
#include "utils.h"
#include "a_star.h"
#include "random.h"
#include "animation/multi_animation_calculator.h"
#include "dungeon/dungeon.h"
#include "hero.h"

Enemy::Enemy() : Entity(Texture(), glm::vec2(0.0f)){
	grid_position = glm::vec2(0.0f);
	walk_path = std::vector<glm::vec2>();
	facing_direction = DOWN;
	_hp = 0;
	animation = nullptr;
	state = CreatureState::Idle;
	should_wait = false;
}

Enemy::Enemy(Texture texture, glm::vec2 grid_start_pos) : Entity(texture, grid_start_pos * SPRITE_WIDTH) {
	grid_position = grid_start_pos;
	_hp = 30;
	walk_path = std::vector<glm::vec2>();
	animation = nullptr;
	state = CreatureState::Idle;
	should_wait = true;
}

Enemy::~Enemy() { }

void Enemy::update(Dungeon &dungeon, float delta_time) {
	if (should_wait)
		return;

	if (state == CreatureState::Attacking || state == CreatureState::Moving) {
		if (animation->has_ended()) {
			state = CreatureState::Idle;
			should_wait = true;

			position = animation->get_animation_position(delta_time);
			grid_position = position / SPRITE_WIDTH;
		}
		else {
			glm::vec2 new_position = animation->get_animation_position(delta_time);
			position = new_position;
		}
	}
	else {
		auto hero = dungeon.hero;
		if (_can_attack(hero->grid_position)) {
			facing_direction = get_direction_from_positions(grid_position, hero->grid_position);
			_attack(*hero);
		}
		else {
			_walk(dungeon);
		}
	}
}

void Enemy::render(SpriteRenderer &renderer) {
	renderer.render(texture, position, get_sprite_rotation(facing_direction));
}

void Enemy::_walk(Dungeon &dungeon) {
	auto hero = dungeon.hero;
	// TODO: Avoid generating new path all the time
	walk_path = generate_enemy_path(dungeon, hero->grid_position);
	auto new_grid_position = walk_path[0];
	auto new_position = new_grid_position * SPRITE_WIDTH;

	facing_direction = get_direction_from_positions(grid_position, new_grid_position);

	if (animation != nullptr) delete animation;
	animation = new AnimationCalculator(position, new_position, 200.0f);
	state = CreatureState::Moving;
}

bool Enemy::_can_attack(glm::vec2 &hero_position) {
	auto diff = grid_position - hero_position;
	auto val = glm::length(glm::abs(diff));
	return val == 1.0;
}

Direction Enemy::get_direction_from_positions(glm::vec2 &old_position, glm::vec2 &new_position) {
	auto diff = new_position - old_position;

	if (diff.x == 1) {
		return RIGHT;
	}
	else if (diff.x == -1) {
		return LEFT;
	}
	if (diff.y == 1) {
		return DOWN;
	}
	else if (diff.y == -1) {
		return UP;
	}
	return facing_direction;
}

std::vector<glm::vec2> Enemy::generate_enemy_path(Dungeon &dungeon, glm::vec2 &hero_position) {
	std::vector<glm::vec2> path = AStar::find_path(dungeon, grid_position, hero_position);
	path.erase(path.begin());
	path.erase(path.end() - 1);

#if DEBUG
	//printf("--------\nNew path:\n");
	//for(auto p : walk_path) {
	//	printf("(%.1f, %.1f) -> ", p.x, p.y);
	//}
	//printf("\n");
#endif

	return path;
}

void Enemy::_attack(Hero &hero) {
	glm::vec2 pixel_position = hero.position;
	auto steps = std::vector<AnimationStep> {
		AnimationStep(position, pixel_position, 150),
		AnimationStep(pixel_position, position, 150)
	};

	if (animation != nullptr) delete animation;
	animation = new MultiAnimationCalculator(steps);
	state = CreatureState::Attacking;

	int r = random_rangei(1, 11);
	hero.take_damage(r);
	printf("Hero HP: %d\n", hero.hp());
}

bool Enemy::check_death() {
	return _hp == 0;
}

int32_t Enemy::hp() {
	return _hp;
}

void Enemy::take_damage(int32_t value) {
	auto tmp = _hp - value;

	if (tmp <= 0) {
		_hp = 0;
	}
	else {
		_hp = tmp;
	}
}

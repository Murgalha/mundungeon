#include <glm/vec2.hpp>
#include "sprite_renderer.h"
#include "enemy.h"
#include "texture.h"
#include "utils.h"
#include "a_star.h"
#include "random.h"
#include "dungeon.h"

Enemy::Enemy() : Entity(Texture(), glm::vec2(0.0f)){
	grid_position = glm::vec2(0.0f);
	walk_path = std::vector<glm::vec2>();
	facing_direction = DOWN;
	hp = 0;
	animation = nullptr;
	is_moving = false;
}

Enemy::Enemy(Texture texture, glm::vec2 grid_start_pos) : Entity(texture, grid_start_pos * SPRITE_WIDTH) {
	grid_position = grid_start_pos;
	hp = 30;
	walk_path = std::vector<glm::vec2>();
	is_moving = false;
	animation = nullptr;
}

Enemy::~Enemy() { }

void Enemy::update(Dungeon &dungeon, float delta_time) {
	if (is_moving) {
		if (animation->has_ended()) {
			is_moving = false;
			position = animation->target;
			grid_position = position / SPRITE_WIDTH;
		}
		else {
			glm::vec2 new_position = animation->get_animation_position(delta_time);
			position = new_position;
		}
	}
	else if (dungeon.turn_action != HeroAction::NoAction) {
		auto hero = dungeon.hero;
		if (_can_attack(hero->grid_position)) {
			facing_direction = get_direction_from_positions(grid_position, hero->grid_position);
			printf("Now we attack.\n");
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
	animation = new AnimationCalculator(position, new_position, 300.0f);
	is_moving = true;
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
	printf("--------\nNew path:\n");
	for(auto p : walk_path) {
		printf("(%.1f, %.1f) -> ", p.x, p.y);
	}
	printf("\n");
#endif

	return path;
}

void Enemy::_attack(Hero &hero) {
	int r = random_rangei(1, 11);

	printf("HP before attack: %d\n", hero.hp);
	hero.hp -= r;
	printf("HP after attack: %d\n", hero.hp);
}

bool Enemy::check_death() {
	return hp <= 0;
}

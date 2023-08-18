#include <glm/vec2.hpp>
#include "sprite_renderer.h"
#include "enemy.h"
#include "texture.h"
#include "utils.h"
#include "a_star.h"
#include "random.h"

Enemy::Enemy() : Entity(-1, glm::vec2(0.0f)){
	grid_position = glm::vec2(0.0f);
	walk_path = std::vector<glm::vec2>();
	facing_direction = DOWN;
	hp = 0;
}

Enemy::Enemy(uint32_t texture, glm::vec2 grid_start_pos) : Entity(texture, grid_start_pos * SPRITE_WIDTH) {
	grid_position = grid_start_pos;
	hp = 30;
	walk_path = std::vector<glm::vec2>();
}

Enemy::~Enemy() { }

void Enemy::render(SpriteRenderer &renderer) {
	glm::vec2 window_position = grid_position * SPRITE_WIDTH;
	renderer.draw_sprite_with_rotation(texture_id, window_position, get_sprite_rotation(facing_direction));
}

void Enemy::walk(Dungeon &dungeon, Hero &hero) {
	if (can_attack(hero.grid_position)) {
		facing_direction = get_direction_from_positions(grid_position, hero.grid_position);
		printf("Now we attack.\n");
		attack(hero);
	}
	else {
		walk_path = generate_enemy_path(dungeon, hero.grid_position);
		auto new_position = walk_path[0];

		facing_direction = get_direction_from_positions(grid_position, new_position);

		grid_position = new_position;
	}
 }

bool Enemy::can_attack(glm::vec2 &hero_position) {
	auto diff = grid_position - hero_position;
	auto val = glm::length(glm::abs(diff));
	printf("Diff: %.1f\n", val);

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

void Enemy::attack(Hero &hero) {
	int r = random_rangei(1, 11);

	printf("HP before attack: %d\n", hero.hp);
	hero.hp -= r;
	printf("HP after attack: %d\n", hero.hp);
}

bool Enemy::check_death() {
	return hp <= 0;
}

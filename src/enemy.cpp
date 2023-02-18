#include <glm/vec2.hpp>
#include "sprite_renderer.h"
#include "enemy.h"
#include "texture.h"
#include "utils.h"
#include "a_star.h"

Enemy::Enemy() {
	texture_id = -1;
	position = glm::vec2(0.0f);
	walk_path = std::vector<glm::vec2>();
	facing_direction = DOWN;
}

Enemy::Enemy(glm::vec2 start_position, Dungeon &dungeon, glm::vec2 &hero_position) {
	texture_id = texture_new((char *)"assets/enemy.png", GL_RGBA, false);
	position = start_position;

	walk_path = generate_enemy_path(dungeon, hero_position);
}

Enemy::~Enemy() { }

void Enemy::draw(SpriteRenderer *renderer) {
	glm::vec2 window_position = position * SPRITE_WIDTH;
	renderer->draw_sprite_with_rotation(texture_id, window_position, get_sprite_rotation(facing_direction));
}

void Enemy::walk(Dungeon &dungeon, glm::vec2 &hero_position) {
	if (can_attack(hero_position)) {
		facing_direction = get_direction_from_positions(position, hero_position);
		printf("Now we attack.\n");
	}
	else {
		walk_path = generate_enemy_path(dungeon, hero_position);
		auto new_position = walk_path[0];

		facing_direction = get_direction_from_positions(position, new_position);

		position = new_position;
	}
 }

bool Enemy::can_attack(glm::vec2 &hero_position) {
	auto diff = position - hero_position;
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
	std::vector<glm::vec2> path = AStar::find_path(dungeon, position, hero_position);
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

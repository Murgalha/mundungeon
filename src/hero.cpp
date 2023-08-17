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
bool can_walk(Dungeon &, glm::vec2 &);
void hero_move(Hero *, Dungeon &, Direction, float);

Hero::Hero() {
	texture_id = texture_new((char *)"assets/hero.png", GL_RGBA, false);
	grid_position = glm::vec2(25.0, 25.0);
	real_position = grid_position * SPRITE_WIDTH;
	hp = 100;
	is_moving = false;
	animation = nullptr;
}

Hero::~Hero() {}

void Hero::attack(Dungeon &dungeon) {
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

void Hero::draw(SpriteRenderer *renderer, float delta_time) {
	renderer->draw_sprite_with_rotation(texture_id, real_position, get_sprite_rotation(facing_direction));
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
			hero_move(this, dungeon, RIGHT, delta_time);
			break;
		case HeroAction::WalkLeft:
			hero_move(this, dungeon, LEFT, delta_time);
			break;
		case HeroAction::WalkDown:
			hero_move(this, dungeon, DOWN, delta_time);
			break;
		case HeroAction::WalkUp:
			hero_move(this, dungeon, UP, delta_time);
			break;
		case HeroAction::Attack:
			attack(dungeon);
			break;
		}
	}
}

void hero_move(Hero *hero, Dungeon &dungeon, Direction d, float delta_time) {
	if (!hero->is_moving) {
		glm::vec2 new_grid_position = hero->grid_position + dir_array[d];

		hero->facing_direction = d;
		if(can_walk(dungeon, new_grid_position)) {
			glm::vec2 pixel_position = new_grid_position * SPRITE_WIDTH;
			hero->is_moving = true;

			if (hero->animation != nullptr) delete hero->animation;
			hero->animation = new AnimationCalculator(hero->real_position, pixel_position, 300.0f);
		}
	}
}

bool can_walk(Dungeon &dungeon, glm::vec2 &position) {
	DungeonTile tile = dungeon.map[(int)position.y][(int)position.x];

	return tile != DungeonTile::Wall &&
		tile != DungeonTile::Empty &&
		dungeon.enemies[(int)position.y][(int)position.x] != 1;
}

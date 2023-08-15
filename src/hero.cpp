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
bool can_walk(Dungeon *, glm::vec2 &);
glm::vec2 _lerp(glm::vec2 &, glm::vec2 &, float);

Hero::Hero() {
	texture_id = texture_new((char *)"assets/hero.png", GL_RGBA, false);
	grid_position = glm::vec2(25.0, 25.0);
	real_position = grid_position * SPRITE_WIDTH;
	hp = 100;
	is_moving = false;
	movement_time = 800.0f;
	elapsed_time = 0.0f;
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

void hero_render(Hero *hero, SpriteRenderer *renderer, float delta_time) {
	if (hero->is_moving) {
		glm::vec2 target_grid_position = hero->grid_position + dir_array[hero->facing_direction];
		glm::vec2 target_position = target_grid_position * SPRITE_WIDTH;

		if (hero->elapsed_time < hero->movement_time) {
			hero->real_position =
				glm::lerp(hero->real_position, target_position, (hero->elapsed_time/hero->movement_time));

			hero->elapsed_time += delta_time;
		}
		else {
			hero->is_moving = false;
			hero->elapsed_time = 0.0f;
			hero->real_position = target_position;
			hero->grid_position = target_grid_position;
		}
	}
	renderer->draw_sprite_with_rotation(hero->texture_id, hero->real_position, get_sprite_rotation(hero->facing_direction));
}

void hero_move(Hero *hero, Dungeon *dungeon, Direction d, Camera *camera, float delta_time) {
	if (!hero->is_moving) {
		glm::vec2 new_position = hero->grid_position + dir_array[d];

		hero->facing_direction = d;
		if(can_walk(dungeon, new_position)) {
			//camera_move(camera, d, delta_time);
			hero->is_moving = true;
		}
	}
}

bool can_walk(Dungeon *dungeon, glm::vec2 &position) {
	DungeonTile tile = dungeon->map[(int)position.y][(int)position.x];

	return tile != DungeonTile::Wall &&
		tile != DungeonTile::Empty &&
		dungeon->enemies[(int)position.y][(int)position.x] != 1;
}

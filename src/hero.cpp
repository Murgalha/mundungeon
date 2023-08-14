#include <stdlib.h>
#include <stdio.h>
#include "hero.h"
#include "texture.h"
#include "dungeon_tile.h"
#include "sprite_renderer.h"
#include "utils.h"
#include "dungeon.h"
#include "random.h"

float get_sprite_rotation(Direction);
bool can_walk(Dungeon *, glm::vec2 &);

Hero::Hero() {
	texture_id = texture_new((char *)"assets/hero.png", GL_RGBA, false);
	position = glm::vec2(25.0, 25.0);
	hp = 100;
}

Hero::~Hero() {}

void Hero::attack(Dungeon &dungeon) {
	auto offset = dir_array[facing_direction];
	auto x = (int)(position[0] + offset[0]);
	auto y = (int)(position[1] + offset[1]);

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

void hero_render(Hero *hero, SpriteRenderer *renderer) {
	glm::vec2 position = hero->position * SPRITE_WIDTH;
	renderer->draw_sprite_with_rotation(hero->texture_id, position, get_sprite_rotation(hero->facing_direction));
}

void hero_move(Hero *hero, Dungeon *dungeon, Direction d, Camera *camera, float delta_time) {
	glm::vec2 new_position = hero->position + dir_array[d];

	hero->facing_direction = d;
	if(can_walk(dungeon, new_position)) {
		hero->position = new_position;
		camera_move(camera, d, delta_time);
	}
}

bool can_walk(Dungeon *dungeon, glm::vec2 &position) {
	DungeonTile tile = dungeon->map[(int)position.y][(int)position.x];

	return tile != DungeonTile::Wall &&
		tile != DungeonTile::Empty &&
		dungeon->enemies[(int)position.y][(int)position.x] != 1;
}

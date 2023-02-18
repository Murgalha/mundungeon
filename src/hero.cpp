#include <stdlib.h>
#include <stdio.h>
#include "hero.h"
#include "texture.h"
#include "dungeon_tile.h"
#include "sprite_renderer.h"
#include "utils.h"
#include "dungeon.h"

float get_sprite_rotation(Direction);
bool can_walk(Dungeon *, glm::vec2 &);

Hero::Hero() {
	texture_id = texture_new((char *)"assets/hero.png", GL_RGBA, false);
	position = glm::vec2(25.0, 25.0);
}

Hero::~Hero() {}

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

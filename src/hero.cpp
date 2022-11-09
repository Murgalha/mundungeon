#include <stdlib.h>
#include <stdio.h>
#include "hero.h"
#include "texture.h"
#include "sprite_renderer.h"
#include "utils.h"

float get_sprite_rotation(Direction);

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
	int x = (int)new_position[0];
	int y = (int)new_position[1];

	char tile = dungeon->map[y][x];
	if(tile != WALL && tile != EMPTY) {
		hero->position = new_position;
		camera_move(camera, d, delta_time);
		hero->facing_direction = d;
	}
}

float get_sprite_rotation(Direction direction) {
	switch(direction) {
	case UP:
		return 0.0f;
		break;
	case DOWN:
		return 180.0f;
		break;
	case LEFT:
		return 270.0f;
		break;
	case RIGHT:
		return 90.0f;
		break;
	default:
		return 0.0f;
	}
}

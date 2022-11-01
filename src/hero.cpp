#include <stdlib.h>
#include <stdio.h>
#include "hero.h"
#include "texture.h"
#include "sprite_renderer.h"
#include "utils.h"

Hero::Hero() {
	this->texture_id = texture_new((char *)"assets/hero.png", GL_RGBA, false);
	vec2 position;
	position[0] = position[1] = 25.0;
	glm_vec2_copy(position, this->position);
}

Hero::~Hero() {}

void hero_render(Hero *hero, SpriteRenderer *renderer) {
	vec2 position;
	glm_vec2_scale(hero->position, SPRITE_WIDTH, position);
	sprite_renderer_draw_sprite(renderer, hero->texture_id, position);
}

void hero_move(Hero *hero, Dungeon *dungeon, Direction d, Camera *camera, float delta_time) {
	int x, y;
	vec2 new_position;

	glm_vec2_add(hero->position, dir_array[d], new_position);
	x = (int)new_position[0];
	y = (int)new_position[1];

	char tile = dungeon->map[y][x];
	if(tile != WALL && tile != EMPTY) {
		glm_vec2_copy(new_position, hero->position);
		camera_move(camera, d, delta_time);
	}
}

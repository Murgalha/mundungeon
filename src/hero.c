#include <stdlib.h>
#include "hero.h"
#include "texture.h"
#include "sprite_renderer.h"

Hero *hero_new() {
	Hero *hero = malloc(sizeof(Hero));
	hero->texture_id = texture_new("assets/hero.png", GL_RGBA, false);
	vec2 position;
	position[0] = position[1] = 25.0;
	glm_vec2_copy(position, hero->position);

	return hero;
}

void hero_delete(Hero *hero) {
	free(hero);
}

void hero_render(Hero *hero, SpriteRenderer *renderer) {
	vec2 size, color, position;
	glm_vec2_scale(hero->position, 128.0, position);
	size[0] = size[1] = 128.0;
	color[0] = color[1] = 1.0;
	sprite_renderer_draw_sprite(renderer, hero->texture_id, position, size, 0.0, color);
}

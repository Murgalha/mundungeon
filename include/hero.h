#ifndef _HERO_H_
#define _HERO_H_

#include <glm/vec2.hpp>
#include "sprite_renderer.h"
#include "camera.h"

struct Dungeon;

struct Hero {
	unsigned int texture_id;
	glm::vec2 position;
	Direction facing_direction;

	Hero();
	~Hero();
};

void hero_render(Hero *, SpriteRenderer *);
void hero_move(Hero *, Dungeon *, Direction, Camera *, float);

#endif

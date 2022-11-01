#ifndef _HERO_H_
#define _HERO_H_

#include <cglm/vec2.h>
#include "types.h"
#include "sprite_renderer.h"
#include "dungeon.h"
#include "camera.h"

struct Hero {
	uint texture_id;
	vec2 position;

	Hero();
	~Hero();
};

void hero_render(Hero *, SpriteRenderer *);
void hero_move(Hero *, Dungeon *, Direction, Camera *, float);

#endif

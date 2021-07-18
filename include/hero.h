#ifndef _HERO_H_
#define _HERO_H_

#include <cglm/vec2.h>
#include "types.h"
#include "utils.h"
#include "sprite_renderer.h"

typedef struct {
	uint texture_id;
	vec2 position;
} Hero;

Hero *hero_new();
void hero_delete(Hero *);
void hero_render(Hero *, SpriteRenderer *);

#endif

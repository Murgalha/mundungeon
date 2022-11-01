#ifndef _SPRITE_RENDERER_H_
#define _SPRITE_RENDERER_H_

#include "types.h"
#include "shader.h"
#include <cglm/vec3.h>
#include <cglm/vec2.h>

struct SpriteRenderer {
	Shader *shader;
	uint quad_VAO;
	uint sprite_width;
	uint sprite_height;

	SpriteRenderer(Shader *);
	~SpriteRenderer();
};

void sprite_renderer_draw_sprite(SpriteRenderer *, uint, vec2);

#endif

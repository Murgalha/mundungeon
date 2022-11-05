#ifndef _SPRITE_RENDERER_H_
#define _SPRITE_RENDERER_H_

#include "types.h"
#include "shader.h"
#include <glm/vec2.hpp>

struct SpriteRenderer {
	Shader *shader;
	uint quad_VAO;
	uint sprite_width;
	uint sprite_height;

	SpriteRenderer(Shader *);
	~SpriteRenderer();
};

void sprite_renderer_draw_sprite(SpriteRenderer *, uint, glm::vec2);

#endif

#ifndef _SPRITE_RENDERER_H_
#define _SPRITE_RENDERER_H_

#include "shader.h"
#include <glm/vec2.hpp>

struct SpriteRenderer {
	Shader *shader;
	unsigned int quad_VAO;
	unsigned int sprite_width;
	unsigned int sprite_height;

	SpriteRenderer(Shader *);
	~SpriteRenderer();
	void draw_sprite(unsigned int, glm::vec2);
	void draw_sprite_with_rotation(unsigned int, glm::vec2, float);
};

#endif

#ifndef _SPRITE_RENDERER_H_
#define _SPRITE_RENDERER_H_

#include "shader.h"
#include "utils.h"
#include "texture.h"
#include <glm/vec2.hpp>
#include <stdint.h>

struct SpriteRenderer {
	Shader *shader;
	unsigned int quad_VAO;
	unsigned int sprite_width;
	unsigned int sprite_height;
	int counter;

	SpriteRenderer(Shader *);
	~SpriteRenderer();
	void render(Texture, glm::vec2, float = 0.0f, glm::vec4 = glm::vec4(1.0f), glm::vec2 = glm::vec2(SPRITE_WIDTH, SPRITE_HEIGHT));
};

#endif

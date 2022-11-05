#ifndef _TEXT_RENDERER_H_
#define _TEXT_RENDERER_H_

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "shader.h"


struct Character {
    unsigned int texture;
	glm::vec2 size;
	glm::vec2 bearing;
    unsigned int advance;
};

struct TextRenderer {
	Character characters[128];
	Shader *shader;
	unsigned int vao;
	unsigned int vbo;

	TextRenderer();
	~TextRenderer();
};

void text_renderer_draw(TextRenderer *, char *, float, float, float, glm::vec3);

#endif

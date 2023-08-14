#ifndef _TEXT_RENDERER_H_
#define _TEXT_RENDERER_H_

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <string>
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
	float bitmap_font_size;

	TextRenderer(glm::mat4 &);
	~TextRenderer();
	void RenderText(std::string, float, float, float, glm::vec3);
	void draw(std::string, glm::vec2, float, glm::vec3);
private:
	int create_characters();
};

void text_renderer_draw(TextRenderer *, char *, float, float, float, glm::vec3);

#endif

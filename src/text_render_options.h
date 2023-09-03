#ifndef _TEXT_RENDER_OPTIONS_H_
#define _TEXT_RENDER_OPTIONS_H_

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "text_alignment.h"

struct TextRenderOptions {
	glm::vec2 position;
	float font_size;
	glm::vec4 color;
	TextAlignment alignment;
};

#endif

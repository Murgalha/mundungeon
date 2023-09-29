#ifndef _TEXT_RENDER_OPTIONS_BUILDER_H_
#define _TEXT_RENDER_OPTIONS_BUILDER_H_

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "text_rendering/text_alignment.h"
#include "text_rendering/text_render_options.h"

struct TextRenderOptionsBuilder {
	TextRenderOptionsBuilder();
	TextRenderOptionsBuilder &with_font_size(float);
	TextRenderOptionsBuilder &with_alignment(TextAlignment);
	TextRenderOptionsBuilder &with_color(glm::vec4);
	TextRenderOptionsBuilder &with_position(glm::vec2);

	TextRenderOptions build();

private:
	TextRenderOptions _options;
};

#endif

#include "text_rendering/text_render_options_builder.h"

TextRenderOptionsBuilder::TextRenderOptionsBuilder() {
	_options.font_size = 14.0f;
	_options.alignment = TextAlignment::Left;
	_options.color = glm::vec4(1.0f);
	_options.position = glm::vec2(0.0f);
}

TextRenderOptionsBuilder& TextRenderOptionsBuilder::with_font_size(float size) {
	_options.font_size = size;
	return *this;
}

TextRenderOptionsBuilder& TextRenderOptionsBuilder::with_alignment(TextAlignment alignment) {
	_options.alignment = alignment;
	return *this;
}

TextRenderOptionsBuilder& TextRenderOptionsBuilder::with_color(glm::vec4 color) {
	_options.color = color;
	return *this;
}

TextRenderOptionsBuilder& TextRenderOptionsBuilder::with_position(glm::vec2 position) {
	_options.position = position;
	return *this;
}

TextRenderOptions TextRenderOptionsBuilder::build() {
	return _options;
}

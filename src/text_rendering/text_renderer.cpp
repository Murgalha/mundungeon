#include <stdio.h>
#include "text_rendering/text_renderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H

TextRenderer::TextRenderer(glm::mat4 &projection) {
	bitmap_font_size = 100.0f;

	shader = new Shader();
	shader->create(GL_VERTEX_SHADER, (char *)"shaders/text.vert");
	shader->create(GL_FRAGMENT_SHADER, (char *)"shaders/text.frag");
	shader->create_program();

	shader->use();
	shader->set_mat4((char *)"projection", projection);

	_create_characters();

	unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	vao = VAO;
	vbo = VBO;
}

TextRenderer::~TextRenderer() {
	delete shader;
}

void TextRenderer::render(std::string text, TextRenderOptions const &options) {
	auto x = options.position.x;
	auto y = options.position.y;
	auto scale = options.font_size / bitmap_font_size;

    shader->use();
	glm::vec3 color3 = glm::vec3(options.color.r, options.color.g, options.color.b);
	shader->set_vec3((char *)"textColor", color3);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao);

	auto offset = _calculate_offset(text, scale, options.alignment);

    // iterate through all characters
    for (long unsigned int i = 0; i != text.size(); i++) {
		char c = text[i];
        Character ch = characters[(int)c];

        float xpos = x + offset.x + ch.bearing.x * scale;
        float ypos = y + offset.y + (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos - h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos - h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos - h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.texture);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

glm::vec2 TextRenderer::_calculate_offset(std::string text, float scale, TextAlignment alignment) {
	auto offset = glm::vec2(0.0f);

	if (alignment == TextAlignment::Left)
		return glm::vec2(0.0f);

	auto direction = alignment == TextAlignment::Right ? 1 : -1;

	for (auto c : text) {
        Character ch = characters[(int)c];

		offset.x += (ch.size.x * scale * direction);
	}

	if (alignment == TextAlignment::Center)
		offset.x = offset.x / 2.0f;

	return offset;
}


int TextRenderer::_create_characters() {
    FT_Library ft;

    if (FT_Init_FreeType(&ft)) {
        printf("ERROR::FREETYPE: Could not init FreeType Library\n");
        return -1;
    }

    const char *font_name = (char *)"assets/fonts/Antonio-bold.ttf";

    FT_Face face;
    if (FT_New_Face(ft, font_name, 0, &face)) {
        printf("ERROR::FREETYPE: Failed to load font\n");
        return -1;
    }

	FT_Set_Pixel_Sizes(face, 0, bitmap_font_size);

	// disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// load first 128 characters of ASCII
	for (unsigned char c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			printf("ERROR::FREETYTPE: Failed to load Glyph\n");
			continue;
		}
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D,
					 0,
					 GL_RED,
					 face->glyph->bitmap.width,
					 face->glyph->bitmap.rows,
					 0,
					 GL_RED,
					 GL_UNSIGNED_BYTE,
					 face->glyph->bitmap.buffer
					 );

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glm::vec2 size;
		size[0] = face->glyph->bitmap.width;
		size[1] = face->glyph->bitmap.rows;
		glm::vec2 bearing;
		bearing[0] = face->glyph->bitmap_left;
		bearing[1] = face->glyph->bitmap_top;

		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<unsigned int>(face->glyph->advance.x)
		};
		characters[c] = character;

	}
	glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

	return 0;
}

#include <stdio.h>
#include <glad/glad.h>
#include "text_renderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H

int create_characters(TextRenderer *renderer) {
    // FreeType
    // --------
    FT_Library ft;
	printf("1\n");
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft)) {
        printf("ERROR::FREETYPE: Could not init FreeType Library\n");
        return -1;
    }

	printf("2\n");
	// find path to font
    char *font_name = (char *)"assets/fonts/OpenSans-Regular.ttf";

	// load font as face
	printf("3\n");
    FT_Face face;
    if (FT_New_Face(ft, font_name, 0, &face)) {
        printf("ERROR::FREETYPE: Failed to load font\n");
        return -1;
    }

	// set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);
	printf("4\n");

	// disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	printf("5\n");

	// load first 128 characters of ASCII
	for (unsigned char c = 0; c < 128; c++) {
		printf("Char: %d\n", (int)c);
		// Load character glyph
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

		renderer->characters[c].texture = texture;
		renderer->characters[c].size[0] = size[0];
		renderer->characters[c].size[1] = size[1];
		renderer->characters[c].bearing[0] = bearing[0];
		renderer->characters[c].bearing[1] = bearing[1];
		renderer->characters[c].advance = face->glyph->advance.x;

	}
	glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

	return 0;
}

TextRenderer::TextRenderer() {
	shader = new Shader();
	shader_create(shader, GL_VERTEX_SHADER, (char *)"shaders/text.vert");
	shader_create(shader, GL_FRAGMENT_SHADER, (char *)"shaders/text.frag");
	shader_create_program(shader);

	create_characters(this);

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
	printf("Vao: %d\tVBO: %d\n", vao, vbo);
}

TextRenderer::~TextRenderer() {
	delete shader;
}

void text_renderer_draw(TextRenderer *renderer, char *text, float x, float y, float scale, glm::vec3 color)
{
    // activate corresponding render state
    shader_use(renderer->shader);
	shader_set_vec3(renderer->shader, (char *)"textColor", color[0], color[1], color[2]);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(renderer->vao);

	int len = strlen(text);
	printf("Text size: %d\n", len);
    // iterate through all characters
    for (int i = 0; i != len; i++) {
		char c = text[i];
		printf("c: %c\n", c);
        Character ch = renderer->characters[(int)c];

        float xpos = x + ch.bearing[0] * scale;
        float ypos = y - (ch.size[1] - ch.bearing[1]) * scale;

        float w = ch.size[0] * scale;
        float h = ch.size[1] * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.texture);
        glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

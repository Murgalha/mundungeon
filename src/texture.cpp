#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "texture.h"

Texture::Texture() {
	id = -1;
}

Texture::Texture(char *filename, GLenum format, bool flip) {
	unsigned int texture;
	int width, height, nchannels;
	unsigned char *data;

	stbi_set_flip_vertically_on_load(flip);
	data = stbi_load(filename, &width, &height, &nchannels, 0);

	if(!data) {
		printf("Could not load '%s'\n", filename);
		id = -1;
		return;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);

	id = texture;
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, id);
}

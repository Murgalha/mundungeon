#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <SDL2/SDL_opengl.h>
#include <stdint.h>

struct Texture {
	uint32_t id;

	Texture();
	Texture(char *, GLenum = GL_RGBA, bool = false);
	void bind();
};

#endif

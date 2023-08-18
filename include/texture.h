#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <SDL2/SDL_opengl.h>
#include <stdint.h>

uint32_t texture_new(char *, GLenum, int);
void texture_bind(uint32_t);

#endif

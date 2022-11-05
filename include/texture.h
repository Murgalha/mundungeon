#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <SDL2/SDL_opengl.h>

unsigned int texture_new(char *, GLenum, int);
void texture_bind(unsigned int);

#endif

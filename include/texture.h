#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <SDL2/SDL_opengl.h>
#include "utils.h"

uint texture_new(char *, GLenum, int);
void texture_bind(uint);

#endif

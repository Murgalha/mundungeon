#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <SDL2/SDL.h>

SDL_Window *window_create(int, int);
void window_destroy(SDL_Window *);

#endif

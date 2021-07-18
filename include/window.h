#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <SDL2/SDL.h>

SDL_Window *window_new(int, int);
void window_delete(SDL_Window *);

#endif

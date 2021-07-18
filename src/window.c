#include "window.h"
#include "log.h"

SDL_Window *window_new(int width, int height) {
	SDL_Window *window = NULL;
	int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

	window = SDL_CreateWindow("Mundungeon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);

	if(!window) {
		log_print(ERROR, "Could not create window: %s\n", SDL_GetError());
	}

	return window;
}

void window_delete(SDL_Window *window) {
	SDL_DestroyWindow(window);
}

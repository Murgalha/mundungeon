#ifndef _APP_H_
#define _APP_H_

#include <glm/mat4x4.hpp>
#include <SDL2/SDL.h>
#include "game.h"

struct App {
	bool should_quit;
	SDL_Window *window;
	SDL_GLContext gl_context;
	glm::mat4 projection;

	App(int, int);
	~App();
	void handle_input(Game *);

private:
	SDL_GLContext gl_context_init(SDL_Window *);
	SDL_Window *window_new(int, int);
	void resize_viewport(int, int);
};

#endif

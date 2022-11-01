#ifndef _APP_H_
#define _APP_H_

#include <cglm/mat4.h>
#include <SDL2/SDL.h>

struct App {
	bool should_quit;
	SDL_Window *window;
	SDL_GLContext gl_context;
	mat4 projection;

	App(int, int);
	~App();

private:
	SDL_GLContext gl_context_init(SDL_Window *);
	SDL_Window *window_new(int, int);
};

#endif

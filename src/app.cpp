#include "app.h"
#include "log.h"

App::App(int window_width, int window_height) {
	window = window_new(window_width, window_height);
	should_quit = false;
	gl_context = gl_context_init(window);
}

App::~App() {
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
}

SDL_Window *App::window_new(int width, int height) {
	SDL_Window *window = NULL;
	int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

	window = SDL_CreateWindow("Mundungeon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);

	if(!window) {
		log_print(ERROR, "Could not create window: %s\n", SDL_GetError());
	}

	return window;
}

SDL_GLContext App::gl_context_init(SDL_Window *window) {
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GLContext context = SDL_GL_CreateContext(window);
	if(!context) {
		log_print(ERROR, "Could not create context: %s\n", SDL_GetError());
		exit(1);
	}

	return context;
}

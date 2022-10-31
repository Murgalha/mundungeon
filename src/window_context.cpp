#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "window_context.h"
#include "window.h"
#define LOG_COLORED_OUTPUT
#include "log.h"

SDL_GLContext gl_context_init(SDL_Window *window) {
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

// TODO: Add arguments to customize the window
WindowContext *window_context_new(int width, int height) {
	WindowContext *ctx = (WindowContext *)malloc(sizeof(WindowContext));
	SDL_Window *window = window_new(width, height);
	SDL_GLContext context = gl_context_init(window);

	ctx->window = window;
	ctx->quit = 0;
	ctx->gl_context = context;
	return ctx;
}

void window_context_delete(WindowContext *ctx) {
	SDL_GL_DeleteContext(ctx->gl_context);
	window_delete(ctx->window);
}

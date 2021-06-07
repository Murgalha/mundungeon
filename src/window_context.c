#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "window_context.h"

WindowContext *window_context_new() {
	WindowContext *ctx = malloc(sizeof(WindowContext));
	ctx->window = ctx->gl_context = NULL;
	ctx->quit = 0;
	return ctx;
}

void window_context_delete(WindowContext *ctx) {
	SDL_GL_DeleteContext(ctx->gl_context);
	SDL_DestroyWindow(ctx->window);
}

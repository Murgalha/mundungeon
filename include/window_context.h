#ifndef _WINDOW_CONTEXT_H_
#define _WINDOW_CONTEXT_H_

#include <cglm/mat4.h>

typedef struct {
	int quit;
	SDL_Window *window;
	SDL_GLContext gl_context;
	mat4 projection;
} WindowContext;

WindowContext *window_context_new(int, int);
void window_context_delete(WindowContext *);

#endif

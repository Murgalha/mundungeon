#include <time.h>
#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>
#include <cglm/cglm.h>
#define LOG_COLORED_OUTPUT
#include "log.h"
#include "window_context.h"
#include "window.h"
#include "texture.h"
#include "texture_atlas.h"
#include "shader.h"
#include "dungeon.h"
#include "random.h"

void resize_viewport(int width, int height) {
	glViewport(0, 0, width, height);
}

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

void process_input(WindowContext *ctx, Shader *s) {
	SDL_Event e;
	while(SDL_PollEvent(&e)) {
		switch(e.type) {
		case SDL_WINDOWEVENT:
			switch(e.window.event) {
			case SDL_WINDOWEVENT_RESIZED:
				resize_viewport(e.window.data1, e.window.data2);
				break;
			default:
				break;
			}
			break;
		case SDL_QUIT:
			ctx->quit = 1;
			break;
		case SDL_KEYDOWN:
			switch(e.key.keysym.sym) {
			case SDLK_RIGHT:
				break;
			case SDLK_LEFT:
				break;
			case SDLK_DOWN:
				break;
			case SDLK_UP:
				break;
			case SDLK_SPACE:
				break;
			case SDLK_F10:
				shader_reload(s);
				break;
			}
			break;
		default:
			break;
		}
	}
}

f32 vertices[] = {
	-0.5, 0.5, 0.0, 1.0,
	-0.5, -0.5, 0.0, 0.0,
	0.5, 0.5, 1.0, 1.0,
	0.5, 0.5, 1.0, 1.0,
	0.5, -0.5, 1.0, 0.0,
	-0.5, -0.5, 0.0, 0.0,
};

int main(int argc, char *argv[]) {
	if(SDL_InitSubSystem(SDL_INIT_EVERYTHING ^ SDL_INIT_HAPTIC)) {
		log_print(ERROR, "Could not initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	WindowContext *window_ctx = window_context_new();
	SDL_Window *window = window_create(800, 600);
	SDL_GLContext context = gl_context_init(window);

	TextureAtlas *texture_atlas;
	Shader *shader = shader_new();


	random_init(time(NULL));
	dungeon_new(50);

	window_ctx->window = window;
	window_ctx->quit = 0;
	window_ctx->gl_context = context;
	glm_ortho(0.0, 800.0, 600.0, 0.0, -1.0, 1.0, window_ctx->projection);

	texture_atlas = texture_atlas_new("assets/hero.png", 16, 16);

	SDL_GL_SetSwapInterval(1);
	glViewport(0, 0, 800, 600);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader_create(shader, GL_VERTEX_SHADER, "shaders/shader.vert");
	shader_create(shader, GL_FRAGMENT_SHADER, "shaders/shader.frag");
	shader_create_program(shader);

	unsigned int VAO, VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Binding stuff
    glBindVertexArray(VAO);

	// Buffering VBO data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

	shader_use(shader);
	shader_set_mat4(shader, "projection", window_ctx->projection);

	mat4 model;
	glm_mat4_identity(model);
	shader_set_mat4(shader, "model", model);

	while(!window_ctx->quit) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		process_input(window_ctx, shader);

		shader_use(shader);
		texture_bind(texture_atlas->texture_id);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

		SDL_GL_SwapWindow(window_ctx->window);
	}

	window_context_delete(window_ctx);
	shader_delete(shader);
	SDL_Quit();

	return 0;
}

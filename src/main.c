#include <time.h>
#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>
#include <cglm/cglm.h>
#include "log.h"
#include "window_context.h"
#include "random.h"
#include "game.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 1024

void resize_viewport(int width, int height) {
	glViewport(0, 0, width, height);
}

void process_input(WindowContext *ctx, Game *game, float delta_time) {
	SDL_Event e;
	mat4 projection;
	float radians;

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
			case SDLK_d:
				camera_process_keyboard(game->camera, RIGHT, delta_time);
				break;
			case SDLK_a:
				camera_process_keyboard(game->camera, LEFT, delta_time);
				break;
			case SDLK_s:
				camera_process_keyboard(game->camera, DOWN, delta_time);
				break;
			case SDLK_w:
				camera_process_keyboard(game->camera, UP, delta_time);
				break;
			case SDLK_EQUALS: // TODO: Fix all zooms
				game->camera->zoom = 0.0f;
				break;
			case SDLK_MINUS:
				game->camera->zoom -= 0.1f;
				break;
			case SDLK_PLUS:
				game->camera->zoom += 0.1f;
				break;
			case SDLK_RIGHT:
				game->hero->position[0]++;
				break;
			case SDLK_LEFT:
				game->hero->position[0]--;
				break;
			case SDLK_DOWN:
				game->hero->position[1]++;
				break;
			case SDLK_UP:
				game->hero->position[1]--;
				break;
			case SDLK_SPACE:
				break;
			}
			break;
		default:
			break;
		}
	}
}

int main(int argc, char *argv[]) {
	if(SDL_InitSubSystem(SDL_INIT_EVERYTHING ^ SDL_INIT_HAPTIC)) {
		log_print(ERROR, "Could not initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	random_init(time(NULL));

	WindowContext *window_ctx = window_context_new(SCREEN_WIDTH, SCREEN_HEIGHT);
	Game *game = game_new(SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_GL_SetSwapInterval(1);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	game_init(game);

    float delta_time = 0.0f;
    float last_frame = 0.0f;
	mat4 view;
	float current_frame;
	while(!window_ctx->quit) {
		current_frame = SDL_GetTicks();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        game_process_input(game, delta_time);
		process_input(window_ctx, game, delta_time);

        game_update(game, delta_time);

		shader_use(game->renderer->shader);
		camera_view_matrix(game->camera, view);
		shader_set_mat4(game->renderer->shader, "view", view);

        glClearColor(0.3f, 0.7f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        game_render(game);

		SDL_GL_SwapWindow(window_ctx->window);
	}

	game_delete(game);
	window_context_delete(window_ctx);
	SDL_Quit();

	return 0;
}

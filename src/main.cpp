#include <time.h>
#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>
#include "app.h"
#include "random.h"
#include "game.h"
#include "clock.h"
#include "utils.h"

#ifdef DEBUG
#include "opengl_debug.h"
#endif

#define UNUSED(X) (void)(X)

int main(int argc, char *argv[]) {
	UNUSED(argc);
	UNUSED(argv);

	if(SDL_InitSubSystem(SDL_INIT_EVERYTHING ^ SDL_INIT_HAPTIC)) {
		printf("Could not initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	//random_init(time(NULL));
	random_init(13);

	App *app = new App(SCREEN_WIDTH, SCREEN_HEIGHT);
	Game *game = new Game(SCREEN_WIDTH, SCREEN_HEIGHT);
	Clock clock = Clock();

	SDL_GL_SetSwapInterval(1);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef DEBUG
	// Enable OpenGL debug
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(message_callback, 0);
#endif

	game->init();

	double delta_time;
	while(!game->should_quit && !app->should_quit) {
		delta_time = clock.delta_time();

		// Handle input and update game
		app->handle_input(game);
        game->update(delta_time);

		// Clear screen and render
        glClearColor(0.165f, 0.114f, 0.31f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        game->render();

		SDL_GL_SwapWindow(app->window);
	}

	delete game;
	delete app;
	SDL_Quit();

	return 0;
}

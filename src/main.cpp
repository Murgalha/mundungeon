#include <time.h>
#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "log.h"
#include "app.h"
#include "random.h"
#include "game.h"
#include "utils.h"

#ifdef DEBUG
#include "opengl_debug.h"
#endif

#define UNUSED(X) (void)(X)

void resize_viewport(int width, int height) {
	glViewport(0, 0, width, height);
}

void process_input(App *app, Game *game, float delta_time) {
	SDL_Event e;

	while(SDL_PollEvent(&e)) {
		ImGui_ImplSDL2_ProcessEvent(&e);

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
			app->should_quit = true;
			break;
		case SDL_KEYDOWN:
			switch(e.key.keysym.sym) {
				// TODO: make camera-only movement as free-roam and not tile-based
			case SDLK_RIGHT:
				camera_move(game->camera, RIGHT, delta_time);
				break;
			case SDLK_LEFT:
				camera_move(game->camera, LEFT, delta_time);
				break;
			case SDLK_DOWN:
				camera_move(game->camera, DOWN, delta_time);
				break;
			case SDLK_UP:
				camera_move(game->camera, UP, delta_time);
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
			case SDLK_d:
				hero_move(game->hero, game->dungeon, RIGHT, game->camera, delta_time);
				break;
			case SDLK_a:
				hero_move(game->hero, game->dungeon, LEFT, game->camera, delta_time);
				break;
			case SDLK_s:
				hero_move(game->hero, game->dungeon, DOWN, game->camera, delta_time);
				break;
			case SDLK_w:
				hero_move(game->hero, game->dungeon, UP, game->camera, delta_time);
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
	UNUSED(argc);
	UNUSED(argv);

	if(SDL_InitSubSystem(SDL_INIT_EVERYTHING ^ SDL_INIT_HAPTIC)) {
		log_print(ERROR, "Could not initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	//random_init(time(NULL));
	random_init(13);

	App *app = new App(SCREEN_WIDTH, SCREEN_HEIGHT);
	Game *game = new Game(SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_GL_SetSwapInterval(1);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef DEBUG
	// Enable OpenGL debug
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(message_callback, 0);
#endif

	const char* glsl_version = "#version 330 core";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(app->window, app->gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

	game_init(game);

    float delta_time = 0.0f;
    float last_frame = 0.0f;
	glm::mat4 view;
	float current_frame;
	while(!app->should_quit) {
		current_frame = SDL_GetTicks();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        game_process_input(game, delta_time);
		process_input(app, game, delta_time);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();

        ImGui::NewFrame();
		ImGui::Begin("Window info");
		ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("Delta time: %.2f ms", delta_time);
		ImGui::End();

        game_update(game, delta_time);

		shader_use(game->sprite_renderer->shader);
		view = camera_view_matrix(game->camera);
		shader_set_mat4(game->sprite_renderer->shader, (char *)"view", view);

        ImGui::Render();
        glClearColor(0.165f, 0.114f, 0.31f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        game_render(game);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(app->window);
	}

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

	delete game;
	delete app;
	SDL_Quit();


	return 0;
}

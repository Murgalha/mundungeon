#include <glm/gtc/matrix_transform.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "game.h"
#include "hero_action.h"

#define UNUSED(X) (void)(X)

Game::Game(unsigned int viewport_width, unsigned int viewport_height) {
	width = viewport_width;
	height = viewport_height;
	keys = (bool *)malloc(sizeof(bool) * 1024);
	state = GAME_ACTIVE;
	sprite_renderer = NULL;
	text_renderer = NULL;
	dungeon = new Dungeon(50);
	camera = new Camera(dungeon->hero->grid_position);

	direction_map = std::map<SDL_Keycode, HeroAction> {
		{ SDLK_UP, HeroAction::WalkUp },
		{ SDLK_LEFT, HeroAction::WalkLeft },
		{ SDLK_DOWN, HeroAction::WalkDown },
		{ SDLK_RIGHT, HeroAction::WalkRight }
	};
}

Game::~Game() {
	delete sprite_renderer;
	delete text_renderer;
	delete camera;
	delete dungeon;
	free(keys);
}

void Game::init() {
	Shader *shader = new Shader();
	shader_create(shader, GL_VERTEX_SHADER, (char *)"shaders/shader.vert");
	shader_create(shader, GL_FRAGMENT_SHADER, (char *)"shaders/shader.frag");
	shader_create_program(shader);

	glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -10.0f, 10.0f);

	shader_use(shader);
	shader_set_int(shader, (char *)"image", 0);
	shader_set_mat4(shader, (char *)"projection", projection);
    sprite_renderer = new SpriteRenderer(shader);
    //game->text_renderer = new TextRenderer();
}

void Game::process_input(SDL_Event e, float delta_time) {
	HeroAction action;
	bool has_moved = false;
	ImGui_ImplSDL2_ProcessEvent(&e);

	switch(e.type) {
	case SDL_KEYDOWN:
		switch(e.key.keysym.sym) {
			// TODO: make camera-only movement as free-roam and not tile-based
			/*
		case SDLK_RIGHT:
			camera_move(camera, RIGHT, delta_time);
			break;
		case SDLK_LEFT:
			camera_move(camera, LEFT, delta_time);
			break;
		case SDLK_DOWN:
			camera_move(camera, DOWN, delta_time);
			break;
		case SDLK_UP:
			camera_move(camera, UP, delta_time);
			break;
			*/
		case SDLK_UP:
			action = HeroAction::WalkUp;
			has_moved = true;
			break;
		case SDLK_DOWN:
			action = HeroAction::WalkDown;
			has_moved = true;
			break;
		case SDLK_LEFT:
			action = HeroAction::WalkLeft;
			has_moved = true;
			break;
		case SDLK_RIGHT:
			action = HeroAction::WalkRight;
			has_moved = true;
			break;
		case SDLK_x:
			action = HeroAction::Attack;
			has_moved = true;
			break;
		case SDLK_SPACE:
			break;
		}
		break;
	default:
		break;
	}

	dungeon->hero->update(action, *dungeon, delta_time);

	if(has_moved) {
		glm::vec2 enemy_pos = dungeon->enemy.position;
		dungeon->enemies[(int)enemy_pos.y][(int)enemy_pos.x] = 0;
		dungeon->enemy.walk(*dungeon, *(dungeon->hero));

		enemy_pos = dungeon->enemy.position;
		dungeon->enemies[(int)enemy_pos.y][(int)enemy_pos.x] = 1;

	}

	dungeon->post_turn_cleanup();
}

void Game::update(float delta_time) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();

        ImGui::NewFrame();
		ImGui::Begin("Window info");
		ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("Delta time: %.2f ms", delta_time);
		ImGui::End();

		shader_use(sprite_renderer->shader);
		auto view = camera_view_matrix(camera);
		shader_set_mat4(sprite_renderer->shader, (char *)"view", view);

}

void Game::render(float delta_time) {
	ImGui::Render();

	//text_renderer_draw(game->text_renderer, (char *)"hello", 0.0, 0.0, 1.0, glm::vec3(1.0f));
	dungeon_render(dungeon, sprite_renderer, delta_time);
}

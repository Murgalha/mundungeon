#include <glm/gtc/matrix_transform.hpp>
#include "game.h"
#include "renderer_repo.h"

#define UNUSED(X) (void)(X)

Game::Game(unsigned int viewport_width, unsigned int viewport_height) {
	width = viewport_width;
	height = viewport_height;
	keys = (bool *)malloc(sizeof(bool) * 1024);
	state = GAME_ACTIVE;
	text_renderer = NULL;
	dungeon = NULL;
	should_quit = false;

	input_map = std::map<SDL_Keycode, Input> {
		{ SDLK_UP, Input::ArrowUp },
		{ SDLK_LEFT, Input::ArrowLeft },
		{ SDLK_DOWN, Input::ArrowDown },
		{ SDLK_RIGHT, Input::ArrowRight },
		{ SDLK_RETURN, Input::Enter },
		{ SDLK_x, Input::X }
	};
}

Game::~Game() {
	delete text_renderer;
	delete dungeon;
	free(keys);
}

void Game::init() {
	Shader *shader = new Shader();
	shader->create((char *)"shaders/default.vert", (char *)"shaders/default.frag");

	Shader *blink_shader = new Shader();
	blink_shader->create((char *)"shaders/default.vert", (char *)"shaders/blink.frag");

	glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -10.0f, 10.0f);

	renderer_repo["default"] = new SpriteRenderer(shader);
	renderer_repo["blink"] = new SpriteRenderer(blink_shader);
	printf("Setting renderers...\n");

	shader->use();
	shader->set_int((char *)"image", 0);
	shader->set_mat4((char *)"projection", projection);

	blink_shader->use();
	blink_shader->set_int((char *)"image", 0);
	blink_shader->set_mat4((char *)"projection", projection);

	text_renderer = new TextRenderer(projection);

	dungeon = new Dungeon(50);
}

bool Game::handle_input(SDL_Event e) {
	Input input = Input::Unknown;
	bool handled = false;

	// TODO: Maybe this translation should be done on App level
	switch(e.type) {
	case SDL_KEYDOWN:
		input = input_map[e.key.keysym.sym];
		handled = dungeon->handle_input(input);
		break;
	default:
		break;
	}

	return handled;
}

void Game::update(float delta_time) {
	if (dungeon->game_over_action == GameOverAction::Quit) {
		should_quit = true;
	}
	else if (dungeon->game_over_action == GameOverAction::Restart) {
		delete dungeon;
		dungeon = new Dungeon(50);
	}
	else {
		dungeon->update(delta_time);

		auto renderer = renderer_repo["default"];
		auto shader = renderer->shader;
		auto b_renderer = renderer_repo["blink"];
		auto blink_shader = b_renderer->shader;
		shader->use();

		auto view = dungeon->camera->view_matrix();
		shader->set_mat4((char *)"view", view);

		blink_shader->use();
		blink_shader->set_mat4((char *)"view", view);
	}
}

void Game::render() {
	dungeon->render(text_renderer);
}

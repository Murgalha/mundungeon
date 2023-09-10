#include <glm/gtc/matrix_transform.hpp>
#include "game.h"

#define UNUSED(X) (void)(X)

Game::Game(unsigned int viewport_width, unsigned int viewport_height) {
	width = viewport_width;
	height = viewport_height;
	keys = (bool *)malloc(sizeof(bool) * 1024);
	state = GAME_ACTIVE;
	sprite_renderer = NULL;
	text_renderer = NULL;
	dungeon = new Dungeon(50);

	input_map = std::map<SDL_Keycode, Input> {
		{ SDLK_UP, Input::MoveUp },
		{ SDLK_LEFT, Input::MoveLeft },
		{ SDLK_DOWN, Input::MoveDown },
		{ SDLK_RIGHT, Input::MoveRight }
	};
}

Game::~Game() {
	delete sprite_renderer;
	delete text_renderer;
	delete dungeon;
	free(keys);
}

void Game::init() {
	Shader *shader = new Shader();
	shader->create(GL_VERTEX_SHADER, (char *)"shaders/shader.vert");
	shader->create(GL_FRAGMENT_SHADER, (char *)"shaders/shader.frag");
	shader->create_program();

	glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -10.0f, 10.0f);

	shader->use();
	shader->set_int((char *)"image", 0);
	shader->set_mat4((char *)"projection", projection);
    sprite_renderer = new SpriteRenderer(shader);
    text_renderer = new TextRenderer(projection);
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
		dungeon->update(delta_time);

		auto shader = sprite_renderer->shader;
		shader->use();

		auto view = dungeon->camera->view_matrix();
		shader->set_mat4((char *)"view", view);
}

void Game::render() {
	dungeon->render(*sprite_renderer, *text_renderer);
}

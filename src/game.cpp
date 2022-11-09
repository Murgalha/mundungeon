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
	hero = new Hero();
	camera = new Camera(hero->position);
}

Game::~Game() {
	delete sprite_renderer;
	delete text_renderer;
	delete camera;
	delete dungeon;
	delete hero;
	free(keys);
}

void game_init(Game *game) {
	Shader *shader = new Shader();
	shader_create(shader, GL_VERTEX_SHADER, (char *)"shaders/shader.vert");
	shader_create(shader, GL_FRAGMENT_SHADER, (char *)"shaders/shader.frag");
	shader_create_program(shader);

	glm::mat4 projection = glm::ortho(0.0f, (float)game->width, (float)game->height, 0.0f, -10.0f, 10.0f);

	shader_use(shader);
	shader_set_int(shader, (char *)"image", 0);
	shader_set_mat4(shader, (char *)"projection", projection);
    game->sprite_renderer = new SpriteRenderer(shader);
    //game->text_renderer = new TextRenderer();
}

void game_process_input(Game *game, float delta_time) {
	UNUSED(game);
	UNUSED(delta_time);
}

void game_update(Game *game, float delta_time) {
	UNUSED(game);
	UNUSED(delta_time);
}

void game_render(Game *game) {
	//text_renderer_draw(game->text_renderer, (char *)"hello", 0.0, 0.0, 1.0, glm::vec3(1.0f));
	dungeon_render(game->dungeon, game->sprite_renderer);
	hero_render(game->hero, game->sprite_renderer);
}

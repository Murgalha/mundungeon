#include <cglm/cam.h>
#include "game.h"

#define UNUSED(X) (void)(X)

Game::Game(uint viewport_width, uint viewport_height) {
	width = viewport_width;
	height = viewport_height;
	keys = (bool *)malloc(sizeof(bool) * 1024);
	state = GAME_ACTIVE;
	renderer = NULL;
	dungeon = new Dungeon(50);
	hero = new Hero();
	camera = new Camera(hero->position);
}

Game::~Game() {
	delete renderer;
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

    mat4 projection;
	glm_ortho(0.0f, (float)game->width, (float)game->height, 0.0f, -10.0f, 10.0f, projection);

	shader_use(shader);
	shader_set_int(shader, (char *)"image", 0);
	shader_set_mat4(shader, (char *)"projection", projection);
    game->renderer = new SpriteRenderer(shader);
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
	dungeon_render(game->dungeon, game->renderer);
	hero_render(game->hero, game->renderer);
}

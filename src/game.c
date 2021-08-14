#include <cglm/cam.h>
#include "game.h"

Game *game_new(uint width, uint height) {
	Game *game = malloc(sizeof(Game));
	game->width = width;
	game->height = height;
	game->keys = malloc(sizeof(bool) * 1024);
	game->state = GAME_ACTIVE;
	game->renderer = NULL;
	game->dungeon = dungeon_new(50);
	game->hero = hero_new();

	vec3 position = GLM_VEC3_ZERO_INIT;
	position[2] = 3.0f;
	game->camera = camera_new(game->hero->position);
	return game;
}

void game_delete(Game *game) {
	sprite_renderer_delete(game->renderer);
	camera_delete(game->camera);
	dungeon_delete(game->dungeon);
	hero_delete(game->hero);
	free(game->keys);
	free(game);
}

void game_init(Game *game) {
	Shader *shader = shader_new();
	shader_create(shader, GL_VERTEX_SHADER, "shaders/shader.vert");
	shader_create(shader, GL_FRAGMENT_SHADER, "shaders/shader.frag");
	shader_create_program(shader);

    mat4 projection;
	glm_ortho(0.0f, (float)game->width, (float)game->height, 0.0f, -10.0f, 10.0f, projection);

	shader_use(shader);
	shader_set_int(shader, "image", 0);
	shader_set_mat4(shader, "projection", projection);
    game->renderer = sprite_renderer_new(shader);
}

void game_process_input(Game *game, float delta_time) {

}

void game_update(Game *game, float delta_time) {

}

void game_render(Game *game) {
	dungeon_render(game->dungeon, game->renderer);
	hero_render(game->hero, game->renderer);
}

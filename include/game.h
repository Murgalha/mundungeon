#ifndef _GAME_H_
#define _GAME_H_

#include "types.h"
#include "shader.h"
#include "sprite_renderer.h"
#include "texture.h"
#include "camera.h"
#include "dungeon.h"
#include "hero.h"

typedef enum {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
} GameState;

typedef struct {
	GameState state;
	bool *keys;
	uint width;
	uint height;
	SpriteRenderer *renderer;
	uint texture_id;
	Camera *camera;
	Dungeon *dungeon;
	Hero *hero;
} Game;

Game *game_new(uint, uint);
void game_delete(Game *);
void game_init(Game *);
void game_process_input(Game *, float);
void game_update(Game *, float);
void game_render(Game *);

#endif

#ifndef _GAME_H_
#define _GAME_H_

#include <SDL2/SDL.h>
#include "types.h"
#include "shader.h"
#include "sprite_renderer.h"
#include "text_rendering/text_renderer.h"
#include "texture.h"
#include "camera.h"
#include "hero.h"
#include "dungeon.h"
#include "hero_action.h"
#include "input.h"

typedef enum {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
} GameState;

struct Game {
	GameState state;
	bool *keys;
	unsigned int width;
	unsigned int height;
	SpriteRenderer *sprite_renderer;
	TextRenderer *text_renderer;
	unsigned int texture_id;
	Camera *camera;
	Dungeon *dungeon;
	std::map<SDL_Keycode, Input> input_map;

	Game(unsigned int, unsigned int);
	~Game();
	bool handle_input(SDL_Event);
	void init();
	void update(float);
	void render();

};

#endif

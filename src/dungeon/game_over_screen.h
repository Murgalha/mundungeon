#ifndef _GAME_OVER_SCREEN_H_
#define _GAME_OVER_SCREEN_H_

#include <glm/vec2.hpp>
#include "input.h"
#include "sprite_renderer.h"
#include "text_rendering/text_renderer.h"

struct GameOverScreen {
	GameOverScreen();
	~GameOverScreen();

	void update(float);
	void render(SpriteRenderer &, TextRenderer &, glm::vec2);
	bool handle_input(Input);

private:
	glm::vec4 background_color, text_color;
	Texture background_tex;
};

#endif

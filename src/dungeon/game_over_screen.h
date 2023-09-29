#ifndef _GAME_OVER_SCREEN_H_
#define _GAME_OVER_SCREEN_H_

#include <vector>
#include <glm/vec2.hpp>
#include "input.h"
#include "sprite_renderer.h"
#include "game_over_action.h"
#include "text_rendering/text_renderer.h"

struct GameOverScreen {
	GameOverScreen();
	~GameOverScreen();

	void update(float);
	void render(SpriteRenderer &sprt_rdr, TextRenderer &text_rdr, glm::vec2 camera);
	bool handle_input(Input input);
	GameOverAction action;
	bool is_closed;

private:
	glm::vec4 background_color, text_color;
	Texture background_tex;
	std::vector<std::pair<std::string, GameOverAction>> menu_items;
	unsigned long selected_item_index;
};

#endif

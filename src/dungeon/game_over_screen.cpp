#include <algorithm>
#include <glm/vec2.hpp>
#include "texture.h"
#include "sprite_renderer.h"
#include "input.h"
#include "text_rendering/text_renderer.h"
#include "text_rendering/text_render_options_builder.h"
#include "dungeon/game_over_screen.h"

#define UNUSED(X) (void)(X)

GameOverScreen::GameOverScreen() {
	background_color = glm::vec4(0.6f, 0.6f, 0.6f, 0.8f); // transparent gray
	text_color = glm::vec4(0.4f, 0.0f, 0.0f, 1.0f); // dark red
	background_tex = Texture((char *) "assets/white.png");

	menu_items = {
		std::make_pair("Restart", GameOverAction::Restart),
		std::make_pair("Quit", GameOverAction::Quit),
	};
	selected_item_index = 0;

	is_closed = false;
	action = GameOverAction::None;
}

GameOverScreen::~GameOverScreen() { }

void GameOverScreen::update(float delta_time) {
	UNUSED(delta_time);
}

void GameOverScreen::render(SpriteRenderer *renderer, TextRenderer *text_renderer, glm::vec2 camera_pos) {
	auto scale = glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT);

	renderer->render(background_tex, camera_pos, 0.0f, background_color, scale);

	glm::vec2 game_over_position = glm::vec2(SCREEN_WIDTH/2 - 30.0, SCREEN_HEIGHT/2 - 100.0f);

	TextRenderOptions game_over_options = TextRenderOptionsBuilder()
		.with_position(game_over_position)
		.with_font_size(40.0)
		.with_color(text_color)
		.with_alignment(TextAlignment::Center)
		.build();

	text_renderer->render("GAME OVER", game_over_options);

	auto item_position = game_over_position;
	item_position.y += 30.0f;

	TextRenderOptionsBuilder menu_items_options_builder = TextRenderOptionsBuilder()
		.with_position(item_position)
		.with_font_size(24.0)
		.with_color(text_color)
		.with_alignment(TextAlignment::Center);

	auto selected_color = glm::vec4(0.9f, 0.0f, 0.0f, 1.0f);

	for (unsigned long i = 0; i < menu_items.size(); i++) {
		auto item = menu_items[i].first;
		auto color = i == selected_item_index ? selected_color : text_color;

		item_position.y+= 30.0f;
		auto options = menu_items_options_builder
			.with_color(color)
			.with_position(item_position)
			.build();

		text_renderer->render(item, options);
	}
}

bool GameOverScreen::handle_input(Input input) {
	bool handled = false;

	switch (input) {
	case Input::ArrowUp:
		selected_item_index = (selected_item_index - 1) % menu_items.size();
		handled = true;
		break;
	case Input::ArrowDown:
		selected_item_index = (selected_item_index + 1) % menu_items.size();
		handled = true;
		break;
	case Input::Enter:
		action = menu_items[selected_item_index].second;
		is_closed = true;
		break;
	default:
		break;
	}

	return handled;
}

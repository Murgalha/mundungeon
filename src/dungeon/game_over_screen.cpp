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
}

GameOverScreen::~GameOverScreen() { }

void GameOverScreen::update(float delta_time) {
	UNUSED(delta_time);
}

void GameOverScreen::render(SpriteRenderer &renderer, TextRenderer &text_renderer, glm::vec2 camera_pos) {
	auto scale = glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT);

	renderer.render(background_tex, camera_pos, 0.0f, background_color, scale);

	glm::vec2 game_over_position = glm::vec2(SCREEN_WIDTH/2 - 30.0, SCREEN_HEIGHT/2);

	TextRenderOptions options = TextRenderOptionsBuilder()
		.with_position(game_over_position)
		.with_font_size(40.0)
		.with_color(text_color)
		.with_alignment(TextAlignment::Center)
		.build();

	text_renderer.render("GAME OVER", options);
}

bool GameOverScreen::handle_input(Input input) {
	UNUSED(input);
	return true;
}

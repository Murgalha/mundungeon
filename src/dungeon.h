#ifndef _DUNGEON_H_
#define _DUNGEON_H_

#include <map>
#include <vector>
#include <glm/vec2.hpp>
#include <cstdint>
#include "sprite_renderer.h"
#include "dungeon_tile.h"
#include "text_renderer.h"
#include "enemy.h"
#include "input.h"
#include "hero_action.h"
#include "texture.h"

struct Dungeon {
	std::map<DungeonTile, Texture> sprites;
	DungeonTile **map;
	unsigned char **enemies;
	Enemy enemy;
	Hero *hero;
	unsigned short size;
	HeroAction turn_action;
	bool is_game_over;

	Dungeon(uint16_t);
	~Dungeon();
	void update(float);
	void render(SpriteRenderer &, TextRenderer &);
	bool handle_input(Input);
	void print();
	bool can_move_to(glm::vec2 &);

private:
	void _create_hero();
	void _post_turn_cleanup();
	void _draw_gameover(SpriteRenderer &, TextRenderer &);
	void _spawn_enemies();
};

#endif
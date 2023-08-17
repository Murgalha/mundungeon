#ifndef _DUNGEON_H_
#define _DUNGEON_H_

#include <map>
#include "sprite_renderer.h"
#include "dungeon_tile.h"
#include <glm/vec2.hpp>
#include "sprite_renderer.h"
#include <vector>
#include "enemy.h"

struct Dungeon {
	std::map<DungeonTile, unsigned int> sprites;
	DungeonTile **map;
	unsigned char **enemies;
	Enemy enemy;
	Hero *hero;
	unsigned short size;
	HeroAction turn_action;

	Dungeon(unsigned short);
	~Dungeon();
	void update(float);
	void post_turn_cleanup();
	void render(SpriteRenderer &);
	void print();
	bool can_move_to(glm::vec2 &);
};

#endif

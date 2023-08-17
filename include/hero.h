#ifndef _HERO_H_
#define _HERO_H_

#include <glm/vec2.hpp>
#include "sprite_renderer.h"
#include "camera.h"
#include "animation_calculator.h"
#include "hero_action.h"

struct Dungeon;

struct Hero {
	unsigned int texture_id;
	glm::vec2 grid_position;
	glm::vec2 real_position;
	Direction facing_direction;
	bool is_moving;
	int hp;
	AnimationCalculator *animation;

	Hero();
	~Hero();
	void update(HeroAction, Dungeon &, float);
	void render(SpriteRenderer &);
private:
	void _attack(Dungeon &);
	void _move(Dungeon &, Direction);
};

#endif

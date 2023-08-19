#ifndef _HERO_H_
#define _HERO_H_

#include <glm/vec2.hpp>
#include "sprite_renderer.h"
#include "camera.h"
#include "animation_calculator.h"
#include "hero_action.h"
#include "entity.h"

struct Dungeon;

struct Hero : public Entity {
	glm::vec2 grid_position;
	Direction facing_direction;
	bool is_moving;
	int hp;
	AnimationCalculator *animation;

	Hero(uint32_t, glm::vec2);
	~Hero();
	void update(Dungeon &, float);
	void render(SpriteRenderer &);
private:
	void _attack(Dungeon &);
	void _move(Dungeon &, Direction);
};

#endif

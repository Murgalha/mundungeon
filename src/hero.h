#ifndef _HERO_H_
#define _HERO_H_

#include <glm/vec2.hpp>
#include "sprite_renderer.h"
#include "animation/animation_calculator.h"
#include "entity.h"
#include "creature_state.h"

struct Dungeon;

struct Hero : public Entity {
	glm::vec2 grid_position;
	Direction facing_direction;
	CreatureState state;
	Animation *animation;

	Hero(Texture, glm::vec2);
	~Hero();
	void update(Dungeon &, float);
	void render();
	bool is_dead();
	void take_damage(int32_t value);
	int32_t hp();

private:
	int32_t _hp;
	void _attack(Dungeon &);
	void _move(Dungeon &, Direction);
};

#endif

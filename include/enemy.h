#ifndef _ENEMY_H_
#define _ENEMY_H_

struct Dungeon;

#include <vector>
#include "types.h"
#include "hero.h"

struct Enemy : public Entity {
	glm::vec2 grid_position;
	Direction facing_direction;
	std::vector<glm::vec2> walk_path;
	int hp;
	AnimationCalculator *animation;
	bool is_moving;

	Enemy();
	Enemy(uint32_t, glm::vec2);
	~Enemy();
	void update(Dungeon &, float);
	void render(SpriteRenderer &);
	bool check_death();

private:
	Direction get_direction_from_positions(glm::vec2 &, glm::vec2 &);
	std::vector<glm::vec2> generate_enemy_path(Dungeon &, glm::vec2 &);
	bool _can_attack(glm::vec2 &);
	void _attack(Hero &);
	void _walk(Dungeon &);
};

#endif

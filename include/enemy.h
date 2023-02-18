#ifndef _ENEMY_H_
#define _ENEMY_H_

struct Dungeon;

#include <vector>
#include "types.h"

struct Enemy {
	unsigned int texture_id;
	glm::vec2 position;
	Direction facing_direction;
	std::vector<glm::vec2> walk_path;

	Enemy();
	Enemy(glm::vec2, Dungeon &, glm::vec2 &);
	~Enemy();
	void draw(SpriteRenderer *);
	void walk(Dungeon &, glm::vec2 &);

private:
	Direction get_direction_from_positions(glm::vec2 &, glm::vec2 &);
	std::vector<glm::vec2> generate_enemy_path(Dungeon &, glm::vec2 &);
	bool can_attack(glm::vec2 &);
};

#endif

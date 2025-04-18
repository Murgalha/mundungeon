#ifndef _ENEMY_H_
#define _ENEMY_H_

struct Dungeon;
struct Hero;

#include <vector>
#include "types.h"
#include "entity.h"
#include "animation/animation.h"
#include "creature_state.h"
#include "duration.h"
#include "sprite_renderer.h"

struct Enemy : public Entity {
	glm::vec2 grid_position;
	Direction facing_direction;
	std::vector<glm::vec2> walk_path;
	Animation *animation;
	CreatureState state;
	bool should_wait;
	SpriteRenderer *renderer;
	Duration damage_duration;

	Enemy();
	Enemy(Texture, glm::vec2);
	~Enemy();
	void update(Dungeon &, float);
	void render();
	bool check_death();
	int32_t hp();
	void take_damage(int32_t value);

private:
	int32_t _hp;
	Direction get_direction_from_positions(glm::vec2 &, glm::vec2 &);
	std::vector<glm::vec2> generate_enemy_path(Dungeon &, glm::vec2 &);
	bool _can_attack(glm::vec2 &);
	void _attack(Hero &);
	void _walk(Dungeon &);
};

#endif

#ifndef _ANIMATION_CALCULATOR_H_
#define _ANIMATION_CALCULATOR_H_

#include <glm/vec2.hpp>
#include "animation/animation_step.h"
#include "animation/animation.h"

struct AnimationCalculator : public Animation {
	float elapsed_time;
	float movement_time;
	glm::vec2 start;
	glm::vec2 target;

	AnimationCalculator(glm::vec2 &, glm::vec2 &, float);
	AnimationCalculator(AnimationStep step);
	AnimationCalculator();
	~AnimationCalculator();
	glm::vec2 get_animation_position(uint64_t delta_time);
	bool has_ended();
};

#endif

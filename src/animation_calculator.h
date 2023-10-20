#ifndef _ANIMATION_CALCULATOR_H_
#define _ANIMATION_CALCULATOR_H_

#include <glm/vec2.hpp>
#include "animation/animation_step.h"

struct AnimationCalculator {
	float elapsed_time;
	float movement_time;
	glm::vec2 start;
	glm::vec2 target;

	AnimationCalculator(glm::vec2 &, glm::vec2 &, float);
	AnimationCalculator(AnimationStep step);
	AnimationCalculator();
	~AnimationCalculator();
	glm::vec2 get_animation_position(float);
	bool has_ended();
};

#endif

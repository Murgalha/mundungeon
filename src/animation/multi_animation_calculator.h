#ifndef _MULTI_ANIMATION_CALCULATOR_H_
#define _MULTI_ANIMATION_CALCULATOR_H_

#include <vector>
#include <stdint.h>
#include "animation/animation_step.h"
#include "animation/animation.h"
#include "animation_calculator.h"

struct MultiAnimationCalculator : public Animation {
	uint64_t elapsed_time;
	uint64_t total_time;
	std::vector<AnimationStep> steps;
	unsigned long step_index;
	AnimationCalculator current;

	MultiAnimationCalculator(std::vector<AnimationStep> &steps);
	~MultiAnimationCalculator();
	glm::vec2 get_animation_position(uint64_t delta_time);
	bool has_ended();
};

#endif

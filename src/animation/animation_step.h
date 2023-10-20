#ifndef _ANIMATION_STEP_H_
#define _ANIMATION_STEP_H_

#include <glm/vec2.hpp>
#include <stdint.h>

struct AnimationStep {
	glm::vec2 start_position, end_position;
	uint64_t duration;

	AnimationStep(glm::vec2 start, glm::vec2 end, uint64_t duration);
};

#endif

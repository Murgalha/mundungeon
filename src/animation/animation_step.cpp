#include "animation/animation_step.h"

AnimationStep::AnimationStep(glm::vec2 start, glm::vec2 end, uint64_t duration) {
	start_position = start;
	end_position = end;
	this->duration = duration;
}

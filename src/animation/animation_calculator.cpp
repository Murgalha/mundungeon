#include <glm/gtx/compatibility.hpp>
#include "animation/animation_calculator.h"

AnimationCalculator::AnimationCalculator(glm::vec2 &start_pos, glm::vec2 &target_pos, float mov_time) {
	start = start_pos;
	target = target_pos;
	elapsed_time = 0.0f;
	movement_time = mov_time;
}

AnimationCalculator::AnimationCalculator(AnimationStep step) {
	start = step.start_position;
	target = step.end_position;
	elapsed_time = 0.0f;
	movement_time = step.duration;
}


AnimationCalculator::AnimationCalculator() {
	start = glm::vec2(0.0f);
	target = glm::vec2(0.0f);
	elapsed_time = 0.0f;
	movement_time = 0.0f;
}

AnimationCalculator::~AnimationCalculator() {}

glm::vec2 AnimationCalculator::get_animation_position(float delta_time) {
	if (elapsed_time > movement_time) {
		return target;
	}

	float step = (elapsed_time/movement_time);
	glm::vec2 new_position = glm::lerp(start, target, step);

	elapsed_time += delta_time;

	return new_position;
}

bool AnimationCalculator::has_ended() {
	return elapsed_time >= movement_time;
}

#include "animation/multi_animation_calculator.h"

MultiAnimationCalculator::MultiAnimationCalculator(std::vector<AnimationStep> &steps) {
	this->steps = steps;
	elapsed_time = 0;
	step_index = 0;

	total_time = 0;
	for (auto step : this->steps) {
		total_time += step.duration;
	}

	current = AnimationCalculator(steps[step_index]);
}

MultiAnimationCalculator::~MultiAnimationCalculator() { }

glm::vec2 MultiAnimationCalculator::get_animation_position(uint64_t delta_time) {
	if (current.has_ended()) {
		step_index++;

		if (has_ended())
			return current.get_animation_position(delta_time);

		current = AnimationCalculator(steps[step_index]);
	}

	return current.get_animation_position(delta_time);
}

inline bool MultiAnimationCalculator::has_ended() {
	return current.has_ended() && step_index >= steps.size();
}

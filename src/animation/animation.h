#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <glm/vec2.hpp>
#include <stdint.h>

struct Animation {
	virtual glm::vec2 get_animation_position(uint64_t delta_time) = 0;
	virtual bool has_ended() = 0;
	virtual ~Animation();
};

#endif

#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <stdint.h>
#include <glm/vec2.hpp>

struct Entity {
	uint32_t texture_id;
	glm::vec2 position;

	Entity(uint32_t, glm::vec2);
};

#endif

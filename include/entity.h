#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <stdint.h>
#include <glm/vec2.hpp>
#include "texture.h"

struct Entity {
	Texture texture;
	glm::vec2 position;

	Entity(Texture, glm::vec2);
};

#endif

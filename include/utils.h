#ifndef _UTILS_H_
#define _UTILS_H_

#include <glm/vec2.hpp>
#include "types.h"

extern glm::vec2 dir_array[4];

float get_sprite_rotation(Direction);

#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 720.0f
#define SPRITE_HEIGHT 32.0f
#define SPRITE_WIDTH 32.0f


#endif

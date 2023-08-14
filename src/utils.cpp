#include "utils.h"

glm::vec2 dir_array[4] = {
	{  0.0, -1.0 }, // UP
	{  0.0,  1.0 }, // DOWN
	{ -1.0,  0.0 }, // LEFT
	{  1.0,  0.0 } // RIGHT
};

float get_sprite_rotation(Direction direction) {
	switch(direction) {
	case UP:
		return 0.0f;
		break;
	case DOWN:
		return 180.0f;
		break;
	case LEFT:
		return 270.0f;
		break;
	case RIGHT:
		return 90.0f;
		break;
	default:
		return 0.0f;
	}
}

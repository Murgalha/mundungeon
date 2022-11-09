#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "types.h"

struct Camera {
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	float zoom;
    float movement_speed;

	Camera(glm::vec2 &);
	~Camera();
};

glm::mat4 camera_view_matrix(Camera *);
void camera_move(Camera *, Direction, float);

#endif

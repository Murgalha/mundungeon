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

	Camera(glm::vec2 &);
	~Camera();
	glm::mat4 view_matrix();
	void focus_on(glm::vec2 &);
};

#endif

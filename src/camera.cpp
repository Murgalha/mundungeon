#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include "sprite_renderer.h"
#include "utils.h"

Camera::Camera(glm::vec2 const &focus_position) {
	position.z = 3.0f;

	up = glm::vec3(0.0f, 1.0f, 0.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	right = glm::normalize(glm::cross(front, up));

	focus_on(focus_position);
}

Camera::~Camera() {}

glm::mat4 Camera::view_matrix() {
	return glm::lookAt(position, position + front, up);
}

void Camera::focus_on(glm::vec2 const &focus_position) {
	position.x = focus_position.x + (SPRITE_WIDTH / 2) - SCREEN_WIDTH / 2;
	position.y = focus_position.y + (SPRITE_HEIGHT / 2) - SCREEN_HEIGHT / 2;
}

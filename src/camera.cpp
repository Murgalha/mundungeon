#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include "sprite_renderer.h"
#include "utils.h"

Camera::Camera(glm::vec2 &hero_position) {
	glm::vec3 position;

	int nsprites_x = SCREEN_WIDTH / SPRITE_WIDTH;
	int nsprites_y = SCREEN_HEIGHT / SPRITE_HEIGHT;

	position[0] = ((hero_position[0] - (nsprites_x / 2)) * SPRITE_WIDTH) + (SPRITE_WIDTH / 2);
	position[1] = ((hero_position[1] - (nsprites_y / 2)) * SPRITE_HEIGHT) + (SPRITE_HEIGHT / 2);
	position[2] = 3.0f;
	this->zoom = 45.0f;

	this->position = position;

	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);

	this->right = glm::normalize(glm::cross(front, up));
	this->movement_speed = 2.5;
}

Camera::~Camera() {}

glm::mat4 camera_view_matrix(Camera *camera) {
	return glm::lookAt(camera->position, camera->position + camera->front, camera->up);
}

void camera_move(Camera *camera, Direction direction, float delta_time) {
	float velocity = camera->movement_speed * delta_time;
	velocity = SPRITE_WIDTH;

	if (direction == UP) {
		// position -= up * velocity
		camera->position -= camera->up * velocity;
	}
	if (direction == DOWN) {
		// position += up * velocity
		camera->position += camera->up * velocity;
	}
	if (direction == LEFT) {
		// position -= right * velocity
		camera->position -= camera->right * velocity;
	}
	if (direction == RIGHT) {
		// position += right * velocity
		camera->position += camera->right * velocity;
	}
}

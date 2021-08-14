#include <cglm/cam.h>
#include "camera.h"
#include "sprite_renderer.h"

Camera *camera_new(vec2 hero_position) {
	Camera *camera = malloc(sizeof(Camera));
	vec3 position;
	// TODO: 6 and 3 are screen-size-dependant, they shouldn't
	position[0] = (hero_position[0] * SPRITE_WIDTH) - 6 * SPRITE_WIDTH;
	position[1] = (hero_position[1] * SPRITE_HEIGHT) - 3 * SPRITE_HEIGHT;
	position[2] = 3.0f;
	camera->zoom = 45.0f;
	glm_vec3_copy(position, camera->position);

	vec3 up = GLM_VEC3_ZERO_INIT;
	up[1] = 1.0f;
	vec3 front = GLM_VEC3_ZERO_INIT;
	front[2] = -1.0f;

	glm_vec3_copy(up, camera->up);
	glm_vec3_copy(front, camera->front);

	glm_vec3_crossn(front, up, camera->right);
	camera->movement_speed = 2.5;

	return camera;
}

void camera_delete(Camera *camera) {
	free(camera);
}

void camera_view_matrix(Camera *camera, mat4 view) {
	vec3 pos_front_sum;

	glm_vec3_add(camera->position, camera->front, pos_front_sum);
	glm_lookat(camera->position, pos_front_sum, camera->up, view);
}

void camera_move(Camera *camera, Direction direction, float delta_time) {
	float velocity = camera->movement_speed * delta_time;
	velocity = SPRITE_WIDTH;
	vec3 vec_product;

	if (direction == UP) {
		// position -= up * velocity
		glm_vec3_scale(camera->up, velocity, vec_product);
		glm_vec3_sub(camera->position, vec_product, camera->position);
	}
	if (direction == DOWN) {
		// position += up * velocity
		glm_vec3_scale(camera->up, velocity, vec_product);
		glm_vec3_add(camera->position, vec_product, camera->position);
	}
	if (direction == LEFT) {
		// position -= right * velocity
		glm_vec3_scale(camera->right, velocity, vec_product);
		glm_vec3_sub(camera->position, vec_product, camera->position);
	}
	if (direction == RIGHT) {
		// position += right * velocity
		glm_vec3_scale(camera->right, velocity, vec_product);
		glm_vec3_add(camera->position, vec_product, camera->position);
	}
}

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <cglm/vec3.h>
#include "types.h"

struct Camera {
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
	float zoom;
    float movement_speed;

	Camera(vec2);
	~Camera();
};

Camera *camera_new(vec2);
void camera_delete(Camera *);
void camera_view_matrix(Camera *, mat4);
void camera_move(Camera *, Direction, float);

#endif

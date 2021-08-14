#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <cglm/vec3.h>
#include "types.h"

typedef struct {
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
	float zoom;
    float movement_speed;
} Camera;

Camera *camera_new(vec2);
void camera_delete(Camera *);
void camera_view_matrix(Camera *, mat4);
void camera_move(Camera *, Direction, float);

#endif

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <cglm/vec3.h>

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} CameraMovement;

typedef struct {
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
	float zoom;
    float movement_speed;
} Camera;

Camera *camera_new(vec3);
void camera_delete(Camera *);
void camera_view_matrix(Camera *, mat4);
void camera_process_keyboard(Camera *, CameraMovement, float);

#endif

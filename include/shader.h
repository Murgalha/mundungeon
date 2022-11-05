#ifndef _SHADER_H_
#define _SHADER_H_

#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>
#include <glm/mat4x4.hpp>
#include "types.h"

struct Shader {
	unsigned int program;
	unsigned int vertex_id;
	unsigned int fragment_id;

	Shader();
	~Shader();
};

void shader_create(Shader *, GLenum, char *);
void shader_create_program(Shader *);
void shader_reload(Shader *);
void shader_use(Shader *);
void shader_set_float(Shader *, char *, float);
void shader_set_int(Shader *, char *, int);
void shader_set_vec3(Shader *, char *, float, float, float);
void shader_set_mat4(Shader *, char *, glm::mat4 &);

#endif

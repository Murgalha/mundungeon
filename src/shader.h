#ifndef _SHADER_H_
#define _SHADER_H_

#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>
#include <glm/mat4x4.hpp>

struct Shader {
	unsigned int program;
	unsigned int vertex_id;
	unsigned int fragment_id;

	Shader();
	~Shader();

	void create(GLenum, char *);
	void create_program();
	void use();
	void set_float(char *, float);
	void set_int(char *, int);
	void set_vec3(char *, glm::vec3 &);
	void set_vec4(char *, glm::vec4 &);
	void set_mat4(char *, glm::mat4 &);
};

#endif

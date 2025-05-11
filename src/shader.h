#ifndef _SHADER_H_
#define _SHADER_H_

#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>
#include <glm/mat4x4.hpp>

struct Shader {
	unsigned int program;

	Shader();
	~Shader();

	void create(char *vs_filename, char *fs_filename);
	void use();
	void set_float(char *, float);
	void set_int(char *, int);
	void set_vec3(char *, glm::vec3 &);
	void set_vec4(char *, glm::vec4 &);
	void set_mat4(char *, glm::mat4 &);

private:
	unsigned int _create(GLenum shader_type, char *filename);
	unsigned int _create_program(unsigned int vertex_id, unsigned int fragment_id);
};

#endif

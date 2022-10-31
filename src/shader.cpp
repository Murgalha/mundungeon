#include <stdlib.h>
#include <stdio.h>
#include <cglm/vec3.h>
#include "shader.h"
#include "file.h"

Shader *shader_new() {
	Shader *s = (Shader *)malloc(sizeof(Shader));
	s->program = 0;
	s->vertex_id = 0;
	s->fragment_id = 0;
	return s;
}

void shader_delete(Shader *s) {
	glDeleteShader(s->vertex_id);
	glDeleteShader(s->fragment_id);
	free(s);
}

void shader_create(Shader *s, GLenum shader_type, char *filename) {
	int success;
	unsigned int shader;
	char info_log[512];
	const char *shader_source = file_read(filename);
	shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &shader_source, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if(!success) {
		glGetShaderInfoLog(shader, 512, NULL, info_log);
		printf("Shader failed to compile: %s\n", info_log);
	}
	if(shader_type == GL_VERTEX_SHADER)
		s->vertex_id = shader;
	else
		s->fragment_id = shader;
}

void shader_create_program(Shader *s) {
	s->program = glCreateProgram();
	glAttachShader(s->program, s->vertex_id);
	glAttachShader(s->program, s->fragment_id);
	glLinkProgram(s->program);
}

void shader_reload(Shader *s) {
	shader_create(s, GL_VERTEX_SHADER, (char *)"shaders/shader.vert");
	shader_create(s, GL_FRAGMENT_SHADER, (char *)"shaders/shader.frag");
	shader_create_program(s);
}

void shader_use(Shader *s) {
	glUseProgram(s->program);
}

void shader_set_float(Shader *s, char *name, float value) {
    glUniform1f(glGetUniformLocation(s->program, name), value);
}

void shader_set_int(Shader *s, char *name, int value) {
    glUniform1i(glGetUniformLocation(s->program, name), value);
}

void shader_set_vec3(Shader *s, char *name, float x, float y, float z) {
	vec3 v;
	v[0] = x; v[1] = y; v[2] = z;
    glUniform3fv(glGetUniformLocation(s->program, name), 1, v);
}

void shader_set_mat4(Shader *s, char *name, mat4 m) {
	unsigned int loc = glGetUniformLocation(s->program, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, *m);
}

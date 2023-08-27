#include <stdlib.h>
#include <stdio.h>
#include "shader.h"
#include "file.h"

Shader::Shader() {
	program = 0;
	vertex_id = 0;
	fragment_id = 0;
}

Shader::~Shader() {
	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);
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
	glm::vec3 v = glm::vec3(x, y, z);
    glUniform3fv(glGetUniformLocation(s->program, name), 1, &v[0]);
}

void shader_set_vec4(Shader *s, char *name, glm::vec4 &v) {
    glUniform4fv(glGetUniformLocation(s->program, name), 1, &v[0]);
}

void shader_set_mat4(Shader *s, char *name, glm::mat4 &m) {
	unsigned int loc = glGetUniformLocation(s->program, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

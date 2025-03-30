#include <stdlib.h>
#include <stdio.h>
#include "shader.h"
#include "file.h"

Shader::Shader() {
	program = 0;
}

Shader::~Shader() {
	glDeleteProgram(program);
}

void Shader::create(char *vs_filename, char *fs_filename) {
	unsigned int vertex_id = _create(GL_VERTEX_SHADER, vs_filename);
	unsigned int fragment_id = _create(GL_FRAGMENT_SHADER, fs_filename);

	program = _create_program(vertex_id, fragment_id);
}

void Shader::use() {
	glUseProgram(program);
}

void Shader::set_float(char *name, float value) {
    glUniform1f(glGetUniformLocation(program, name), value);
}

void Shader::set_int(char *name, int value) {
    glUniform1i(glGetUniformLocation(program, name), value);
}

void Shader::set_vec3(char *name, glm::vec3 &v) {
    glUniform3fv(glGetUniformLocation(program, name), 1, &v[0]);
}

void Shader::set_vec4(char *name, glm::vec4 &v) {
    glUniform4fv(glGetUniformLocation(program, name), 1, &v[0]);
}

void Shader::set_mat4(char *name, glm::mat4 &m) {
	unsigned int loc = glGetUniformLocation(program, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

unsigned int Shader::_create(GLenum shader_type, char *filename) {
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

	return shader;
}

unsigned int Shader::_create_program(unsigned int vertex_id, unsigned int fragment_id) {
	unsigned int program = glCreateProgram();
	glAttachShader(program, vertex_id);
	glAttachShader(program, fragment_id);
	glLinkProgram(program);

	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);

	return program;
}

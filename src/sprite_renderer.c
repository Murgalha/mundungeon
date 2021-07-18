#include <cglm/util.h>
#include <cglm/affine.h>
#include "sprite_renderer.h"
#include "texture.h"

SpriteRenderer *sprite_renderer_new(Shader *shader) {
	SpriteRenderer *renderer = malloc(sizeof(SpriteRenderer));
	renderer->shader = shader;

    unsigned int VBO;
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &(renderer->quad_VAO));
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(renderer->quad_VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	return renderer;
}

void sprite_renderer_delete(SpriteRenderer *renderer) {
    glDeleteVertexArrays(1, &(renderer->quad_VAO));
	shader_delete(renderer->shader);
	free(renderer);
}

void sprite_renderer_draw_sprite(SpriteRenderer *renderer, uint texture_id, vec2 position, vec2 size, float rotate, vec3 color) {
	shader_use(renderer->shader);

    mat4 model = GLM_MAT4_IDENTITY_INIT;
	vec3 pos = GLM_VEC3_ZERO_INIT;
	pos[0] = position[0]; pos[1] = position[1];

    glm_translate(model, pos);

	vec3 to_origin = GLM_VEC3_ZERO_INIT;
	to_origin[0] = size[0] * 0.5;
	to_origin[1] = size[1] * 0.5;
    glm_translate(model, to_origin);

	vec3 zup = GLM_VEC3_ZERO_INIT;
	zup[2] = 1.0;
	float radians = glm_rad(rotate);
	glm_rotate(model, radians, zup);

	vec3 from_origin = GLM_VEC3_ZERO_INIT;
	from_origin[0] = size[0] * -0.5;
	from_origin[1] = size[1] * -0.5;
    glm_translate(model, from_origin);

	vec3 v3_size = GLM_VEC3_ONE_INIT;
	v3_size[0] = size[0];
	v3_size[1] = size[1];
    glm_scale(model, v3_size);

	shader_set_mat4(renderer->shader, "model", model);
	shader_set_vec3(renderer->shader, "spriteColor", color[0], color[1], color[2]);

    glActiveTexture(GL_TEXTURE0);
    texture_bind(texture_id);

    glBindVertexArray(renderer->quad_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
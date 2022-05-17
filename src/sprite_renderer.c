#include <cglm/util.h>
#include <cglm/affine.h>
#include "sprite_renderer.h"
#include "texture.h"

SpriteRenderer *sprite_renderer_new(Shader *shader) {
	SpriteRenderer *renderer = malloc(sizeof(SpriteRenderer));
	renderer->shader = shader;
	renderer->sprite_width = SPRITE_WIDTH;
	renderer->sprite_height = SPRITE_HEIGHT;
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

void sprite_renderer_draw_sprite(SpriteRenderer *renderer, uint texture_id, vec2 position) {
	shader_use(renderer->shader);
	vec2 size;
	size[0] = renderer->sprite_width;
	size[1] = renderer->sprite_height;

	mat4 model = GLM_MAT4_IDENTITY_INIT;
	vec3 pos = GLM_VEC3_ZERO_INIT;
	pos[0] = position[0]; pos[1] = position[1];

    glm_translate(model, pos);

	vec3 v3_size = GLM_VEC3_ONE_INIT;
	v3_size[0] = size[0];
	v3_size[1] = size[1];
    glm_scale(model, v3_size);

	shader_set_mat4(renderer->shader, "model", model);

    glActiveTexture(GL_TEXTURE0);
    texture_bind(texture_id);

    glBindVertexArray(renderer->quad_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

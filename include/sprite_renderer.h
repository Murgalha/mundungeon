#ifndef _SPRITE_RENDERER_H_
#define _SPRITE_RENDERER_H_

#include "types.h"
#include "shader.h"
#include <cglm/vec3.h>
#include <cglm/vec2.h>

typedef struct {
	Shader *shader;
	uint quad_VAO;
} SpriteRenderer;

SpriteRenderer *sprite_renderer_new(Shader *);
void sprite_renderer_delete(SpriteRenderer *);
void sprite_renderer_draw_sprite(SpriteRenderer *, uint, vec2, vec2, float, vec3);

#endif

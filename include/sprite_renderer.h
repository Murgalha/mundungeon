#ifndef _SPRITE_RENDERER_H_
#define _SPRITE_RENDERER_H_

#include "types.h"
#include "shader.h"
#include <cglm/vec3.h>
#include <cglm/vec2.h>

#define SPRITE_HEIGHT 100.0
#define SPRITE_WIDTH 100.0

typedef struct {
	Shader *shader;
	uint quad_VAO;
	uint sprite_width;
	uint sprite_height;
} SpriteRenderer;

SpriteRenderer *sprite_renderer_new(Shader *);
void sprite_renderer_delete(SpriteRenderer *);
void sprite_renderer_draw_sprite(SpriteRenderer *, uint, vec2);

#endif

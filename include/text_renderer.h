#ifndef _TEXT_RENDERER_H_
#define _TEXT_RENDERER_H_

#include <cglm/vec2.h>
#include "shader.h"


typedef struct {
    unsigned int texture;
    vec2 size;
    vec2 bearing;
    unsigned int advance;
} Character;

typedef struct {
	Character characters[128];
	Shader *shader;
	unsigned int vao;
	unsigned int vbo;
} TextRenderer;

TextRenderer *text_renderer_new();
void text_renderer_draw(TextRenderer *, char *, float, float, float, vec3);


#endif

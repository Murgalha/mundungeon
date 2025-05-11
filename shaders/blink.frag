#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform float time;

void main() {
    vec4 tex = texture(image, TexCoords);
    color = vec4(1.0) * tex;
	color.rgb = vec3(1.0);
	color.a *= sin(time / 5.0);
}

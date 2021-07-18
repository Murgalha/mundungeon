#include <stdlib.h>
#include <stdio.h>
#include "dungeon.h"
#include "dungeon_generator.h"
#include "texture.h"

void dungeon_print(Dungeon *dungeon) {
	for(int y = 0; y < dungeon->size; y++) {
		for(int x = 0; x < dungeon->size; x++) {
			printf("%c", dungeon->map[y][x]);
		}
		printf("\n");
	}
}

Dungeon *dungeon_new(u16 size) {
	Dungeon *dungeon = malloc(sizeof(Dungeon));
	dungeon->map = dungeon_generator_new_map(size);
	dungeon->size = size;
	dungeon->door_texture = texture_new("assets/door.png", GL_RGBA, false);
	dungeon->floor_texture = texture_new("assets/floor.png", GL_RGBA, false);
	dungeon->wall_texture = texture_new("assets/wall.png", GL_RGBA, false);

	return dungeon;
}

void dungeon_delete(Dungeon *dungeon) {
	for(int i = 0; i < dungeon->size; i++) {
		free(dungeon->map[i]);
	}
	free(dungeon->map);
	free(dungeon);
}

void dungeon_render(Dungeon *dungeon, SpriteRenderer *renderer) {
	char tile;
	unsigned int texture;
	vec3 color;
	vec2 position;
	vec2 size;
	size[0] = size[1] = 128.0f;
	color[0] = color[1] = color[2] = 1.0f;

	for(int y = 0; y < dungeon->size; y++) {
		for(int x = 0; x < dungeon->size; x++) {
			tile = dungeon->map[y][x];
			position[0] = x * size[0];
			position[1] = y * size[1];

			switch(tile) {
			case 0:
				texture = dungeon->wall_texture;
				color[0] = color[1] = color[2] = 1.0f;
				break;
			case 1:
				texture = dungeon->floor_texture;
				color[0] = color[1] = color[2] = 1.0f;
				break;
			case 2:
				texture = dungeon->door_texture;
				color[0] = color[1] = color[2] = 1.0f;
				break;
			default:
				texture = dungeon->wall_texture;
				color[0] = color[1] = color[2] = 0.0f;
				break;
			}
			sprite_renderer_draw_sprite(renderer, texture, position, size, 0.0, color);
		}
	}
}

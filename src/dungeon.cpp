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

Dungeon::Dungeon(u16 dungeon_size) {
	size = dungeon_size;
	map = DungeonGenerator::new_map(size);
	door_texture = texture_new((char *)"assets/door.png", GL_RGBA, false);
	floor_texture = texture_new((char *)"assets/floor.png", GL_RGBA, false);
	wall_texture = texture_new((char *)"assets/wall.png", GL_RGBA, false);
}

Dungeon::~Dungeon() {
	for(int i = 0; i < size; i++) {
		free(map[i]);
	}
	free(map);
}

void dungeon_render(Dungeon *dungeon, SpriteRenderer *renderer) {
	char tile;
	unsigned int texture;
	glm::vec3 color;
	glm::vec2 position;
	color[0] = color[1] = color[2] = 1.0f;

	for(int y = 0; y < dungeon->size; y++) {
		for(int x = 0; x < dungeon->size; x++) {
			tile = dungeon->map[y][x];
			position[0] = x * renderer->sprite_width;
			position[1] = y * renderer->sprite_height;

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
			sprite_renderer_draw_sprite(renderer, texture, position);
		}
	}
}

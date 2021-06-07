#include <stdlib.h>
#include "texture_atlas.h"
#include "texture.h"

TextureAtlas *texture_atlas_new(char *filename, u16 tile_width, u16 tile_height) {
	TextureAtlas *texture_atlas = malloc(sizeof(TextureAtlas));
	texture_atlas->texture_id = texture_new(filename, GL_RGBA, true);
	texture_atlas->tile_width_px = tile_width;
	texture_atlas->tile_height_px = tile_height;

	return texture_atlas;
}

void texture_atlas_delete(TextureAtlas *texture_atlas) {
	free(texture_atlas);
}

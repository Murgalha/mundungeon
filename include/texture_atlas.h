#ifndef _TEXTURE_ATLAS_H_
#define _TEXTURE_ATLAS_H_

#include "utils.h"

typedef struct {
	uint texture_id;
	u16 tile_width_px;
	u16 tile_height_px;
} TextureAtlas;

TextureAtlas *texture_atlas_new(char *, u16, u16);
void texture_atlas_delete(TextureAtlas *);

#endif

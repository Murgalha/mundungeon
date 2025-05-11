#ifndef _RENDERER_REPO_H_
#define _RENDERER_REPO_H_

#include "sprite_renderer.h"
#include <string>
#include <unordered_map>

extern std::unordered_map<std::string, SpriteRenderer *> renderer_repo;

#endif

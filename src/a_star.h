#ifndef _A_STAR_H_
#define _A_STAR_H_

struct AStar {
	static std::vector<glm::vec2> find_path(Dungeon &dungeon, glm::vec2 &from, glm::vec2 &to);
};

#endif

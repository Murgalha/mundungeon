#include "hero.h"
#include "dungeon.h"
#include <glm/vec2.hpp>
#include <vector>
#include "a_star.h"
#include <array>
#include <cfloat>
#include "dungeon_tile.h"
#include <iostream>
#include <stack>
#include <algorithm>

struct Node {
	int y;
	int x;
	int parentX;
	int parentY;
	float gCost;
	float hCost;
	float fCost;
};

bool node_cmp(const Node &a, const Node &b) {
	return a.fCost < b.fCost;
}

float mhtndist(glm::vec2 v1, glm::vec2 v2) {
	return std::abs(v1.x - v2.x) + std::abs(v1.y - v2.y);
}

bool is_walkable(Dungeon &dungeon, glm::vec2 v) {
	int x = (int)v.x;
	int y = (int)v.y;
	DungeonTile tile = dungeon.map[y][x];

	return (tile == DungeonTile::Floor ||
			tile == DungeonTile::Door ||
			tile == DungeonTile::Corridor) &&
		dungeon.enemies[y][x] == 0;
}

bool are_same_position(glm::vec2 v1, glm::vec2 v2) {
	return v1.x == v2.x && v1.y == v2.y;
}

void init_data(Dungeon &dungeon,
			   std::vector<std::vector<Node>> &nodes,
			   std::vector<std::vector<bool>> &visited_nodes) {
	nodes = std::vector<std::vector<Node>>(dungeon.size, std::vector<Node>(dungeon.size));
	visited_nodes = std::vector<std::vector<bool>>(dungeon.size, std::vector<bool>(dungeon.size));

	for (int y = 0; y < dungeon.size; y++) {
		for (int x = 0; x < dungeon.size; x++) {
			nodes[y][x].fCost = FLT_MAX;
			nodes[y][x].gCost = FLT_MAX;
			nodes[y][x].hCost = FLT_MAX;
			nodes[y][x].parentX = -1;
			nodes[y][x].parentY = -1;
			nodes[y][x].x = x;
			nodes[y][x].y = y;

			visited_nodes[y][x] = false;
		}
	}
}

std::vector<glm::vec2> makePath(std::vector<std::vector<Node>> map, glm::vec2 dest) {
	int x = dest.x;
	int y = dest.y;
	std::stack<glm::vec2> path;
	std::vector<glm::vec2> usablePath;

	while (x != -1 && y != -1)
	{
		Node node = map[y][x];
		path.push(glm::vec2(node.x, node.y));
		x = node.parentX;
		y = node.parentY;
	}

	while (!path.empty()) {
		glm::vec2 top = path.top();
		path.pop();
		usablePath.emplace_back(top);
	}

	return usablePath;
}

std::vector<glm::vec2> AStar::find_path(Dungeon &dungeon, glm::vec2 &start, glm::vec2 &goal) {
	std::vector<glm::vec2> empty;

#ifdef DEBUG
	printf("Starting A* from (%.1f, %.1f) to (%.1f, %.1f)\n", start[0], start[1], goal[0], goal[1]);
#endif

	if (are_same_position(start, goal)) {
		printf("Start and goal are the same\n");
		return empty;
	}

	std::vector<std::vector<bool>> visited_nodes;
	std::vector<std::vector<Node>> nodes;

	init_data(dungeon, nodes, visited_nodes);

	int x = start.x;
	int y = start.y;
	nodes[y][x].fCost = 0.0;
	nodes[y][x].gCost = 0.0;
	nodes[y][x].hCost = 0.0;
	nodes[y][x].parentX = -1;
	nodes[y][x].parentY = -1;


	std::vector<Node> queue; // TODO: Use mean heap to search for data
	queue.emplace_back(nodes[y][x]);
	bool destinationFound = false;

	glm::vec2 neighbor_offset[] = {
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, -1.0f),
		glm::vec2(-1.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
	};

	while (!queue.empty()) {
		auto node_it = std::min_element(queue.begin(), queue.end(), node_cmp);
		Node node = *node_it;

		queue.erase(node_it);
		x = node.x;
		y = node.y;

		visited_nodes[y][x] = true;
		for(auto offset : neighbor_offset) {
			int new_x = x + offset[0];
			int new_y = y + offset[1];
			Node &neighbor = nodes[new_y][new_x];
			double gNew = 0;
			double hNew = 0;
			double fNew = 0;

			if (is_walkable(dungeon, glm::vec2(new_x, new_y))) {

				if (are_same_position(glm::vec2(new_x, new_y), goal)) {
					printf("Found destination\n");
					neighbor.parentX = x;
					neighbor.parentY = y;
					destinationFound = true;
					return makePath(nodes, goal);
				}
				else if (!(visited_nodes[new_y][new_x])) {
					gNew = node.gCost + 1.0;
					hNew = mhtndist(glm::vec2(new_x, new_y), goal);
					fNew = gNew + hNew;

					if (fNew < neighbor.fCost) {
						neighbor.fCost = fNew;
						neighbor.gCost = gNew;
						neighbor.hCost = hNew;
						neighbor.parentX = x;
						neighbor.parentY = y;
						queue.emplace_back(neighbor);
					}
				}
			}
		}
	}
	if (destinationFound == false) {
		printf("Destination not found\n");
		return empty;
	}
	return empty;
}

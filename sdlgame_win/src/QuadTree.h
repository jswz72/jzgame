#pragma once

#include <vector>
#include <SDL.h>
#include "ECS/ECS.h"

class QuadTree {
public:
	QuadTree(int lvl, SDL_Rect bnds) : level(lvl), bounds(bnds) { }
	
	void clear();
	void insert(Entity* entity);
	std::vector<Entity*> retrieve(std::vector<Entity*>& returnEntities, SDL_Rect rect);
private:
	int maxObjects = 10;
	int maxLevels = 5;
	int level;
	std::vector<Entity*> entities;
	SDL_Rect bounds;
	QuadTree* nodes[4] = { nullptr, nullptr, nullptr, nullptr };

	void split();
	int getIndex(SDL_Rect rect);
};
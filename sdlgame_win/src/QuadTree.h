#pragma once

#include <vector>
#include <SDL.h>

class ColliderComponent;

class QuadTree {
public:
	QuadTree(int lvl, SDL_Rect bnds) : level(lvl), bounds(bnds) { }

	void clear();
	void insert(ColliderComponent* collComponent);
	std::vector<ColliderComponent*> retrieve(std::vector<ColliderComponent*>& returnColliders,
		SDL_Rect rect);
private:
	int maxObjects = 10;
	int maxLevels = 5;
	int level;
	std::vector<ColliderComponent*> colliders;
	SDL_Rect bounds;
	QuadTree* nodes[4] = { nullptr, nullptr, nullptr, nullptr };

	void split();
	int getIndex(SDL_Rect rect);
};
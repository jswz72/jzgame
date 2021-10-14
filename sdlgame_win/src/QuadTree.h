#pragma once

#include <vector>
#include <SDL.h>

class ColliderComponent;

class QuadTree {
public:
	QuadTree(int lvl, SDL_Rect bnds) : level(lvl), bounds(bnds) { }
	~QuadTree() {
		for (int i = 0; i < 4; i++) {
			delete nodes[i];
		}
	}

	void clear();
	void insert(ColliderComponent* collComponent);
	std::vector<ColliderComponent*> retrieve(std::vector<ColliderComponent*>& returnColliders,
		SDL_Rect rect);
	QuadTree* nodes[4] = { nullptr, nullptr, nullptr, nullptr };
	SDL_Rect bounds;
private:
	int maxObjects = 10;
	int maxLevels = 5;
	int level;
	std::vector<ColliderComponent*> colliders;

	void split();
	int getIndex(SDL_Rect rect);
	std::vector<int> getIndices(SDL_Rect rect);
};
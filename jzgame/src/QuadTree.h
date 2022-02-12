#pragma once

#include <vector>
#include "Rect.h"

class ColliderComponent;

class QuadTree {
public:
	QuadTree(int lvl, Rect bnds) : level(lvl), bounds(bnds) { }
	~QuadTree() {
		for (int i = 0; i < 4; i++) {
			delete nodes[i];
		}
	}

	void clear();
	void insert(ColliderComponent* collComponent);
	std::vector<ColliderComponent*> retrieve(std::vector<ColliderComponent*>& returnColliders,
		const Rect &rect) const;
	QuadTree* nodes[4] = { nullptr, nullptr, nullptr, nullptr };
	Rect bounds;
private:
	int maxObjects = 10;
	int maxLevels = 5;
	int level;
	std::vector<ColliderComponent*> colliders;

	void split();
	int getIndex(Rect rect);
	std::vector<int> getIndices(Rect rect) const;
};
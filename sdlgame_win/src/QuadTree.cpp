#include "QuadTree.h"
#include "ECS/Components.h"
#include <cassert>

void QuadTree::clear() {
	colliders.clear();
	for (auto i = 0; i < 4; i++) {
		if (nodes[i]) {
			nodes[i]->clear();
			delete nodes[i];
			nodes[i] = nullptr;
		}
	}
}

void QuadTree::split() {
	int subWidth = bounds.w / 2;
	int subHeight = bounds.h / 2;

	nodes[0] = new QuadTree(level + 1, SDL_Rect{ bounds.x + subWidth, bounds.y, subWidth,
												 subHeight });
	nodes[1] = new QuadTree(level + 1, SDL_Rect{ bounds.x + subWidth, bounds.y + subHeight,
												 subWidth, subHeight });
	nodes[2] = new QuadTree(level + 1, SDL_Rect{ bounds.x, bounds.y + subHeight, subWidth,
												 subHeight });
	nodes[3] = new QuadTree(level + 1, SDL_Rect{ bounds.x, bounds.y, subWidth, subHeight });
}

int QuadTree::getIndex(SDL_Rect rect) {
	int index = -1;
	int horMidpoint = bounds.x + (bounds.w / 2);
	int vertMidpoint = bounds.y + (bounds.h / 2);

	bool fitTopHalf = (rect.y < vertMidpoint) && (rect.y + rect.h < vertMidpoint);
	bool fitBotHalf = rect.y > vertMidpoint;
	bool fitLeftHalf = (rect.x < horMidpoint) && (rect.x + rect.w < horMidpoint);
	bool fitRightHalf = rect.x > horMidpoint;

	if (fitTopHalf) {
		if (fitRightHalf) {
			index = 0;
		}
		else if (fitLeftHalf) {
			index = 3;
		}
	}
	else if (fitBotHalf) {
		if (fitRightHalf) {
			index = 1;
		}
		else if (fitLeftHalf) {
			index = 2;
		}
	}
	return index;
}

std::vector<int> QuadTree::getIndices(SDL_Rect rect) {
	std::vector<int> indices;
	int horMidpoint = bounds.x + (bounds.w / 2);
	int vertMidpoint = bounds.y + (bounds.h / 2);

	bool touchingTopHalf = rect.y < vertMidpoint;
	bool touchingBotHalf = rect.y + rect.h > vertMidpoint;
	bool touchingLeftHalf = rect.x < horMidpoint;
	bool touchingRightHalf = rect.x + rect.w > horMidpoint;

	if (touchingTopHalf) {
		if (touchingRightHalf) {
			indices.push_back(0);
		}
		if (touchingLeftHalf) {
			indices.push_back(3);
		}
	}
	if (touchingBotHalf) {
		if (touchingRightHalf) {
			indices.push_back(1);
		}
		if (touchingLeftHalf) {
			indices.push_back(2);
		}
	}
	return indices;
}

void QuadTree::insert(ColliderComponent* collComponent) {
	assert(collComponent->collider.x >= bounds.x);
	assert(collComponent->collider.y >= bounds.y);
	if (nodes[0]) {
		int index = getIndex(collComponent->collider);
		if (index != -1) {
			nodes[index]->insert(collComponent);
			return;
		}
	}
	colliders.push_back(collComponent);
	if (colliders.size() > maxObjects && level < maxLevels) {
		if (!nodes[0]) {
			split();
		}
		int i = 0;
		while (i < colliders.size()) {
			auto entity = colliders[i];
			SDL_Rect col = entity->collider;
			int index = getIndex(col);
			if (index != -1) {
				colliders.erase(colliders.begin() + i);
				nodes[index]->insert(entity);
			}
			else {
				i++;
			}
		}
	}
}

std::vector<ColliderComponent*> QuadTree::retrieve(std::vector<ColliderComponent*>& returnColliders,
	SDL_Rect rect) {
	for (auto& index : getIndices(rect)) {
		if (nodes[index]) {
			nodes[index]->retrieve(returnColliders, rect);
		}
	}
	returnColliders.insert(returnColliders.end(), colliders.begin(), colliders.end());
	return returnColliders;
}
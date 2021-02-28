#include "QuadTree.h"
#include "ECS/Components.h"

void QuadTree::clear() {
	entities.clear();
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
		} else if (fitLeftHalf) {
			index = 3;
		}
	} else if (fitBotHalf) {
		if (fitRightHalf) {
			index = 1;
		} else if (fitLeftHalf) {
			index = 2;
		}
	}
	return index;
}

void QuadTree::insert(Entity* entity) {
	if (nodes[0]) {
		int index = getIndex(entity->getComponent<ColliderComponent>().collider);
		if (index != -1) {
			nodes[index]->insert(entity);
			return;
		}
	}
	entities.push_back(entity);
	if (entities.size() > maxObjects && level < maxLevels) {
		if (!nodes[0]) {
			split();
		}
		int i = 0;
		while (i < entities.size()) {
			auto entity = entities[i];
			SDL_Rect col = entity->getComponent<ColliderComponent>().collider;
			int index = getIndex(col);
			if (index != -1) {
				entities.erase(entities.begin() + i);
				nodes[index]->insert(entity);
			} else {
				i++;
			}
		}
	}
}

std::vector<Entity*> QuadTree::retrieve(std::vector<Entity*>& returnEntities,
										SDL_Rect rect) {
	int index = getIndex(rect);
	if (index != -1 && nodes[0]) {
		nodes[index]->retrieve(returnEntities, rect);
	}
	returnEntities.insert(returnEntities.end(), entities.begin(), entities.end());
	return returnEntities;
}


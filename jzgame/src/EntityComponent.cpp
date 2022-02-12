#include "EntityComponent.h"
#include "Components/Components.h"

void Entity::addGroup(GroupLabel group) {
	groupBitset[static_cast<int>(group)] = true;
	entityManager.addToGroup(this, group);
}

void Entity::delGroup(GroupLabel group) {
	groupBitset[static_cast<int>(group)] = false;
	entityManager.delFromGroup(this, group);
}

void Entity::setTag(std::string newTag) {
	entityManager.updateTag(tag, newTag, this);
	tag = newTag;
}

void EntityManager::refresh() {
	for (auto iter = taggedEntities.begin(); iter != taggedEntities.end();) {
		if (!iter->second->isActive()) {
			iter = taggedEntities.erase(iter);
		}
		else {
			iter++;
		}
	}
	for (int group = 0; group < MAX_GROUPS; group++) {
		auto& groupVector = groupedEntities[group];
		groupVector.erase(std::remove_if(std::begin(groupVector), std::end(groupVector),
			[group](Entity* entity) {
				return !entity->isActive() || !entity->hasGroup(group);
			}), std::end(groupVector));
	}
	colliders.erase(std::remove_if(std::begin(colliders), std::end(colliders),
		[](const ColliderComponent* cc) {
			return !cc->entity->isActive();
		}),
		std::end(colliders));
	entities.erase(std::remove_if(std::begin(entities), std::end(entities),
		[](const std::unique_ptr<Entity>& mEntity) {
			return !mEntity->isActive();
		}),
		std::end(entities));
}
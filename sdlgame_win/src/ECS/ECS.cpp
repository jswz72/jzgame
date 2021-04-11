#include "ECS.h"
#include "Components.h"

void Entity::addGroup(Group group) {
	groupBitset[group] = true;
	manager.addToGroup(this, group);
}

void Entity::setTag(std::string newTag) {
	manager.updateTag(tag, newTag, this);
	tag = newTag;
}

void Manager::refresh() {
	for (auto iter = taggedEntities.begin(); iter != taggedEntities.end();) {
		if (!iter->second->isActive()) {
			iter = taggedEntities.erase(iter);
		}
		else {
			iter++;
		}
	}
	for (auto i = 0u; i < maxGroups; i++) {
		auto& v(groupedEntities[i]);
		v.erase(std::remove_if(std::begin(v), std::end(v),
			[i](Entity* entity) {
				return !entity->isActive() || !entity->hasGroup(i);
			}), std::end(v));
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
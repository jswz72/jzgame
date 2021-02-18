#include "ECS.h"

void Entity::addGroup(Group group) {
	groupBitset[group] = true;
	manager.addToGroup(this, group);
}

void Entity::setTag(std::string newTag) {
	manager.updateTag(tag, newTag, this);
	tag = newTag;
}

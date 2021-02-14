#include "ECS.h"

void Entity::addGroup(Group group) {
	groupBitset[group] = true;
	manager.addToGroup(this, group);
}
#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>
#include "../GroupLabel.h"

#ifndef ECS
#define ECS

using ComponentID = unsigned int;

class Component;
class Entity;
class EntityManager;
class ColliderComponent;

constexpr std::size_t MAX_COMPONENTS = 32;
constexpr std::size_t MAX_GROUPS = 32;

class Component {
public:
	Entity* entity = nullptr;
	virtual void update() {}
	virtual void draw() {}
	virtual ~Component() {}

	template <typename T>
	static ComponentID getComponentTypeID() noexcept {
		static_assert(std::is_base_of<Component, T>::value, "");
		static ComponentID typeID = getNewComponentTypeID();
		return typeID;
	}
private:
	static ComponentID getNewComponentTypeID() {
		static ComponentID lastID = 0;
		return lastID++;
	}
};

class Entity {
public:
	Entity(EntityManager& em) : entityManager(em) {}

	void update() {
		// Calling update on each component may perhaps result in new components, so cannot
		// use iterators here as they may be invalided;
		auto oldSize = components.size();
		for (auto i = 0; i < oldSize; i++) {
			components[i]->update();
		}
	}
	void draw() {
		for (auto& component : components) {
			component->draw();
		}
	}
	bool isActive() const { return active; }
	void destroy() { active = false; }
	bool hasGroup(unsigned int i) {
		return groupBitset[i];
	}
	bool hasGroup(GroupLabel group) {
		return hasGroup(static_cast<int>(group));
	}
	void addGroup(GroupLabel group);
	void delGroup(GroupLabel group);
	void setTag(std::string newTag);
	std::string getTag() {
		return tag;
	}
	template <typename T>
	bool hasComponent() const {
		auto typeID = Component::getComponentTypeID<T>();
		return componentBitset[typeID];
	}
	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... args) {
		T* component = new T(std::forward<TArgs>(args)...);
		std::unique_ptr<Component> uPtrComponent{ component };
		component->entity = this;
		components.emplace_back(std::move(uPtrComponent));

		componentArray[Component::getComponentTypeID<T>()] = component;
		componentBitset[Component::getComponentTypeID<T>()] = true;
		return *component;
	}

	template <typename T>
	T& getComponent() const {
		auto componentPtr = componentArray[Component::getComponentTypeID<T>()];
		// We static cast it downwards to a pointer to whatever component type it is and
		// return a reference.
		return *static_cast<T*>(componentPtr);
	}
private:
	std::string tag = "";
	EntityManager& entityManager;
	bool active = true;
	std::vector<std::unique_ptr<Component>> components{};
	std::array<Component*, MAX_COMPONENTS> componentArray{};
	std::bitset<MAX_COMPONENTS> componentBitset;
	std::bitset<MAX_GROUPS> groupBitset;
};

class EntityManager {
public:
	std::vector<std::unique_ptr<Entity>> entities;
	EntityManager(std::vector<ColliderComponent*>& colls) : colliders(colls) {}
	void update() {
		// Calling update in entities may result in new entities being created so
		// cannot use iterators here as they may become invalidated.
		auto oldSize = entities.size();
		for (auto i = 0; i < oldSize; i++) {
			entities[i]->update();
		}
	}
	void draw() {
		for (auto& entity : entities) {
			entity->draw();
		}
	}
	void refresh();
	void addToGroup(Entity* entity, GroupLabel group) {
		groupedEntities[static_cast<int>(group)].emplace_back(entity);
	}
	void delFromGroup(Entity* entity, GroupLabel group) {
		auto& groupVector = groupedEntities[static_cast<int>(group)];
		groupVector.erase(std::remove(groupVector.begin(),
									  groupVector.end(),
									  entity),
						  groupVector.end());
	}
	void updateTag(std::string oldTag, std::string newTag, Entity* entity) {
		if (taggedEntities.find(oldTag) != taggedEntities.end()) {
			taggedEntities.erase(oldTag);
		}
		taggedEntities[newTag] = entity;
	}
	Entity* getEntityWithTag(std::string tag) {
		if (taggedEntities.find(tag) != taggedEntities.end()) {
			return taggedEntities[tag];
		}
		return nullptr;
	}

	std::vector<Entity*>& getGroup(GroupLabel group) {
		return groupedEntities[static_cast<int>(group)];
	}

	Entity& addEntity() {
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr(e);
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
private:
	std::unordered_map<std::string, Entity*> taggedEntities;
	std::array<std::vector<Entity*>, MAX_GROUPS> groupedEntities;
	std::vector<ColliderComponent*>& colliders;
};

#endif
#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#ifndef ECS
#define ECS

using ComponentID = unsigned int;
using Group = std::size_t;

class Component;
class Entity;
class Manager;

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitset = std::bitset<maxComponents>;
using GroupBitset = std::bitset<maxGroups>;

class Component {
public:
	Entity* entity;
	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}
	virtual ~Component() {}

	template <typename T> static ComponentID getComponentTypeID() noexcept {
		// static_assert used to assert at compile time.
		static_assert (std::is_base_of<Component, T>::value, "");
		// static used, so first typeId returned for each instance of this template (per type) only
		// calls getComponentTypeID once (initially).
		static ComponentID typeID = getNewComponentTypeID();
		return typeID;
	}
private:
	// Inline because defined in header.
	// Creates a typeId for the component. Private so random callers cannot increment typeId
	// without creating type.
	static ComponentID getNewComponentTypeID() {
		// Increasing typeId each call.
		static ComponentID lastID = 0;
		return lastID++;
	}
};

class Entity {
public:
	Entity(Manager& mManager) : manager(mManager) {}

	void update() {
		for (auto& component : components) {
			component->update();
		}
	}
	void draw() {
		for (auto& component : components) {
			component->draw();
		}
	}
	bool isActive() const { return active; }
	void destroy() { active = false; }

	bool hasGroup(Group mGroup) {
		return groupBitset[mGroup];
	}

	void addGroup(Group mGroup);
	void delGroup(Group mGroup) {
		groupBitset[mGroup] = false;
	}
	void setTag(std::string newTag);
	std::string getTag() {
		return tag;
	}

	template <typename T> bool hasComponent() const {
		auto typeID = Component::getComponentTypeID<T>();
		return componentBitset[typeID];
	}
	// TODO
	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs) {
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[Component::getComponentTypeID<T>()] = c;
		componentBitset[Component::getComponentTypeID<T>()] = true;
		c->init();
		return *c;
	}
	// TODO
	template <typename T> T& getComponent() const {
		// At this point it is a Component pointer.
		auto componentPtr(componentArray[Component::getComponentTypeID<T>()]);
		// We static cast it downwards to a pointer to whatever component type it is and
		// return a reference.
		return *static_cast<T*>(componentPtr);
	}
private:
	std::string tag = "";
	Manager& manager;
	bool active = true;
	std::vector<std::unique_ptr<Component>> components{};
	std::array<Component*, maxComponents> componentArray{};
	ComponentBitset componentBitset;
	GroupBitset groupBitset;
};

class Manager {
public:
	void update() {
		for (auto& entity : entities) {
			entity->update();
		}
	}
	void draw() {
		for (auto& entity : entities) {
			entity->draw();
		}
	}
	void refresh() {
		for (auto iter = taggedEntities.begin(); iter != taggedEntities.end();) {
			if (!iter->second->isActive()) {
				iter = taggedEntities.erase(iter);
			} else {
				iter++;
			}
		}
		for (auto i(0u); i < maxGroups; i++) {
			auto& v(groupedEntities[i]);
			v.erase(std::remove_if(std::begin(v), std::end(v),
				[i](Entity* entity) {
					return !entity->isActive() || !entity->hasGroup(i);
				}), std::end(v));
		}
		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity>& mEntity) {
				return !mEntity->isActive();
			}),
			std::end(entities));
	}

	void addToGroup(Entity* entity, Group group) {
		groupedEntities[group].emplace_back(entity);
	}

	void updateTag(std::string oldTag, std::string newTag, Entity* entity) {
		if (taggedEntities.find(oldTag) != taggedEntities.end()) {
			taggedEntities.erase(oldTag);
		}
		taggedEntities[newTag] = entity;
	}

	Entity* getEntityWithTag(std::string tag) {
		return taggedEntities[tag];
	}

	std::vector<Entity*>& getGroup(Group group) {
		return groupedEntities[group];
	}

	Entity& addEntity() {
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr(e);
		// Need to move unique_ptr because cannot be copied. Also more performant.
		// Vector has ownership, so on refresh if the entity is erased, reference returned by
		// this function is no longer valid.
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::unordered_map<std::string, Entity*> taggedEntities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;
};

#endif
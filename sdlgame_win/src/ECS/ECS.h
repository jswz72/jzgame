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
	Entity* entity = nullptr;
	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}
	virtual ~Component() {}

	template <typename T> static ComponentID getComponentTypeID() noexcept {
		// static_assert used to assert at compile time.
		static_assert (std::is_base_of<Component, T>::value, "");
		static ComponentID typeID = getNewComponentTypeID();
		return typeID;
	}
private:
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
	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs) {
		// "A pattern followed by an ellipsis, in which the name of at least one parameter pack
		// appears at least once, is expanded into zero or more comma-separated instantiations
		// of the pattern, where the name of the parameter pack is replaced by each of the
		// elements from the pack, in order" - cppreference.com
		T* component = new T(std::forward<TArgs>(mArgs)...);
		component->entity = this;
		std::unique_ptr<Component> uPtrComponent{ component };
		components.emplace_back(std::move(uPtrComponent));

		componentArray[Component::getComponentTypeID<T>()] = component;
		componentBitset[Component::getComponentTypeID<T>()] = true;
		component->init();
		return *component;
	}

	template <typename T> T& getComponent() const {
		auto componentPtr = componentArray[Component::getComponentTypeID<T>()];
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
	std::vector<std::unique_ptr<Entity>> entities;

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
	void refresh() {
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
		if (taggedEntities.find(tag) != taggedEntities.end()) {
			return taggedEntities[tag];
		}
		return nullptr;
	}

	std::vector<Entity*>& getGroup(Group group) {
		return groupedEntities[group];
	}

	Entity& addEntity() {
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr(e);
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
private:
	std::unordered_map<std::string, Entity*> taggedEntities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;
};

#endif
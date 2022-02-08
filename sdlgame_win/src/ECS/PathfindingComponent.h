#pragma once
#include <stack>
#include <vector>
#include "ECS.h"
#include "TransformComponent.h"
#include "../Map.h"
#include "../Vector2D.h"

const unsigned int DEFAULT_PATHFINDING_PERIOD = 10;


class PathfindingComponent : public Component {
public:
	// Use map to direct this entity's transform.velocity towards the goalPos.
	PathfindingComponent(TransformComponent& transform,
		const Map& map, Vector2D<> goalPos) : transform(transform), map(map), goalPos(goalPos) {
		computePath();
	}
	void update() override;
	void draw() override;
	void setGoal(const Vector2D<> newGoal) {
		goalPos = newGoal;
	}
	void forcePathComputation() {
		computePath();
	}
private:
	TransformComponent& transform;
	const Map& map;
	Vector2D<> goalPos;
	std::vector<Vector2D<int>> path;
	// Regularity that pathfinding should be re-run.
	unsigned int period = DEFAULT_PATHFINDING_PERIOD;
	unsigned int cycles = 0;

	void directVelocity();
	void computePath();
};
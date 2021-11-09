#include "PathfindingComponent.h"

#include <functional>
#include <queue>
#include <unordered_map>
#include "../Collision.h"
#include "../Utils.h"


void PathfindingComponent::directVelocity() {
	assert(transform);
	if (path.empty()) {
		return;
	}
	auto targetCoords = path.top();
	auto target = map->getScaledTile(targetCoords);
	if (!Collision::AABB(target, transform->getRect())) {
		// Have not yet reached next target.
		return;
	}
	path.pop();
	if (path.empty()) {
		return;
	}
	targetCoords = path.top();
	target = map->getScaledTile(targetCoords);
	const auto targetPos = Vector2D(target.x, target.y);
	// Set velocity to reach next target.
	transform->velocity = Utils::directionBetween(transform->getPosition(), targetPos);
}

void PathfindingComponent::update()
{
	directVelocity();
	if (++cycles < period) {
		return;
	}
	cycles = 0;
	computePath();
}

void PathfindingComponent::computePath() {
	assert(transform);
	const auto myCoords = map->getCoords(transform->getPosition());
	assert(myCoords.x >= 0 && myCoords.y >= 0);
	const auto goalCoords = map->getCoords(goal);
	assert(goalCoords.x >= 0 && goalCoords.y >= 0);

	using PQElement = std::pair<unsigned int, Vector2D>;
	auto pqCompare = [](const PQElement& a, const PQElement& b) {
		return a.first > b.first;
	};
	std::priority_queue<PQElement, std::vector<PQElement>, decltype(pqCompare)> queue(pqCompare);
	queue.emplace(0, myCoords);

	std::unordered_map<Vector2D, std::optional<Vector2D>, Vector2D::HashFunction> cameFrom;
	std::unordered_map<Vector2D, unsigned int, Vector2D::HashFunction> costSoFar;
	cameFrom[myCoords] = std::nullopt;
	costSoFar[myCoords] = 0;
	while (!queue.empty()) {
		const auto current = queue.top().second;
		queue.pop();
		if (current == goalCoords) {
			break;
		}
		for (const auto& next : map->neighborCoords(current)) {
			/*if (costSoFar.find(current) == costSoFar.end()) {
				auto f = 1 + 1;
			}*/
			const auto newCost = costSoFar[current] + map->navMap[next.y][next.x];
			if (costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next]) {
				costSoFar[next] = newCost;
				queue.emplace(newCost, next);
				cameFrom[next] = current;
			}
		}
	}
	
	path.push(goalCoords);
	auto nxt = cameFrom[goalCoords];
	while (nxt != std::nullopt) {
		path.push(*nxt);
		nxt = cameFrom[*nxt];
	}
	const auto f = path.top();
	assert( f == myCoords);
}

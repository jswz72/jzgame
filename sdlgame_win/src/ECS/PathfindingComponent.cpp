#include "PathfindingComponent.h"

#include <functional>
#include <limits>
#include <queue>
#include <unordered_map>
#include "../Collision.h"
#include "../Globals.h"
#include "../RGBVals.h"
#include "../Utils.h"

using Coordinates = Vector2D<int>;

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
	const auto targetPos = Vector2D<>(target.x, target.y);
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
	const auto mapBounds = map->getBounds();
	assert(myCoords.x >= 0 && myCoords.x <= mapBounds.x && myCoords.y >= 0 &&
		myCoords.y <= mapBounds.y);
	auto goalCoords = map->getCoords(goalPos);
	assert(goalCoords.x >= 0 && goalCoords.x <= mapBounds.x && goalCoords.y >= 0
	    && goalCoords.y <= mapBounds.y);
	if (myCoords == goalCoords) {
		return;
	}

	using PQElement = std::pair<uint32_t, Coordinates>;
	auto pqCompare = [](const PQElement& a, const PQElement& b) {
		return a.first > b.first;
	};
	std::priority_queue<PQElement, std::vector<PQElement>, decltype(pqCompare)> queue(pqCompare);
	queue.emplace(0, myCoords);

	std::unordered_map<Coordinates, std::optional<Coordinates>, Coordinates::HashFunction> cameFrom;
	std::unordered_map<Coordinates, uint32_t, Coordinates::HashFunction> costSoFar;
	cameFrom[myCoords] = std::nullopt;
	costSoFar[myCoords] = 0;
	while (!queue.empty()) {
		const auto current = queue.top().second;
		queue.pop();
		if (current == goalCoords) {
			break;
		}
		for (const auto& next : map->neighborCoords(current)) {
			// navVal of 0 denotes non-navigatable tile.
			const auto navVal = map->navMap[next.y][next.x];
			if (!navVal) {
				continue;
			}
			const auto newCost = costSoFar.at(current) + navVal;
			const auto nxtCostIter = costSoFar.find(next);
			if (nxtCostIter == costSoFar.end() || newCost < nxtCostIter->second) {
				costSoFar[next] = newCost;
				queue.emplace(newCost, next);
				cameFrom[next] = current;
			}
		}
	}
	
	while (!path.empty()) {
		path.pop();
	}
	// TODO: acceptable?
	// Find next best goal.
	if (!cameFrom.count(goalCoords)) {
		float shortestDist = std::numeric_limits<float>::max();
		auto newGoalCoords = goalCoords;
		for (const auto& iter : cameFrom) {
			const auto& coords = iter.first;
			const int dist = Utils::distance<int>(coords, goalCoords);
			if (dist < shortestDist) {
				shortestDist = dist;
				newGoalCoords = coords;
			}
		}
		goalCoords = newGoalCoords;
	}
	path.push(goalCoords);
	auto nxt = cameFrom.at(goalCoords);
	int i = 0;
	while (nxt != std::nullopt) {
		path.push(*nxt);
		nxt = cameFrom.at(*nxt);
		i++;
	}
	const auto f = path.top();
	assert(f == myCoords);
}

void PathfindingComponent::draw() {
	if (!Globals::get().debug) {
		return;
	}
	std::stack<Coordinates> holder;
	/*for (int i = 0; i < map->navMap.size(); i++) {
		for (int j = 0; j < map->navMap[i].size(); j++) {
			auto rect = Globals::get().cameraRelative(map->getScaledTile(Vector2D<>(j, i)));
			const auto color = map->navMap[i][j] ? RGBVals::white() : RGBVals::black();
			Utils::drawRect(&rect, color);
		}
	}*/
	auto rect = Globals::get().cameraRelative(map->getScaledTile(map->getCoords(goalPos)));
	Utils::drawRect(&rect, RGBVals::blue());
	while(!path.empty()) {
		const auto coords = path.top();
		path.pop();
		holder.push(coords);
		auto rect = Globals::get().cameraRelative(map->getScaledTile(coords));
		Utils::drawRect(&rect, RGBVals::purple());
	}
	while (!holder.empty()) {
		path.push(holder.top());
		holder.pop();
	}
}

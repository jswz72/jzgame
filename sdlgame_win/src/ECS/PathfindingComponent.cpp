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

bool PathfindingComponent::atGoal() {
	const auto goalRect = map.getScaledTile(goalCoords);
	return Collision::AABB(goalRect, transform.getRect());
}

// Direct transform.velocity towards the next coordinate in path.
void PathfindingComponent::directVelocity() {
	if (path.empty()) {
		return;
	}
	auto targetCoords = path.back();
	auto targetRect = map.getScaledTile(targetCoords);
	// TODO: possibly could have moved past target?
	if (!Collision::AABB(targetRect, transform.getRect())) {
		// Have not yet reached next target.
		return;
	}
	path.pop_back();
	if (path.empty()) {
		if (atGoal()) {
			transform.velocity.zero();
		}
		return;
	}
	targetCoords = path.back();
	targetRect = map.getScaledTile(targetCoords);
	const auto targetPos = Vector2D<>(static_cast<float>(targetRect.x),
									  static_cast<float>(targetRect.y));
	// Set velocity to reach next target.
	transform.velocity = Utils::directionBetween(transform.getPosition(), targetPos);
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
	const auto myCoords = map.getCoords(transform.getPosition());
	const auto mapBounds = map.getBounds();
	const auto mapBoundsRect = SDL_Rect{ 0,0,mapBounds.x, mapBounds.y };
	assert(Utils::containedIn(myCoords, mapBoundsRect));
	assert(Utils::containedIn(goalCoords, mapBoundsRect));

	if (atGoal()) {
		return;
	}

	using PQElement = std::pair<uint32_t, Coordinates>;
	auto pqCompare = [](const PQElement& a, const PQElement& b) {
		return a.first > b.first;
	};
	std::priority_queue<PQElement, std::vector<PQElement>, decltype(pqCompare)> queue{pqCompare};
	queue.emplace(0, myCoords);

	std::unordered_map<Coordinates, std::optional<Coordinates>, Coordinates::HashFunction> cameFrom;
	std::unordered_map<Coordinates, uint32_t, Coordinates::HashFunction> costSoFar;
	cameFrom[myCoords] = std::nullopt;
	costSoFar[myCoords] = 0;
	while (!queue.empty()) {
		const auto currentCoords = queue.top().second;
		queue.pop();
		if (currentCoords == goalCoords) {
			break;
		}
		for (const auto& next : map.neighborCoords(currentCoords)) {
			// navVal of 0 denotes non-navigatable tile.
			const auto navVal = map.navMap[next.y][next.x];
			if (!navVal) {
				continue;
			}
			const auto newCost = costSoFar.at(currentCoords) + navVal;
			if (!costSoFar.count(next) || newCost < costSoFar[next]) {
				costSoFar[next] = newCost;
				const auto priority = newCost + Utils::distance(currentCoords, goalCoords);
				queue.emplace(priority, next);
				cameFrom[next] = currentCoords;
			}
		}
	}
	
	// Find next best goal (closest to goal).
	if (!cameFrom.count(goalCoords)) {
		int shortestDist = std::numeric_limits<int>::max();
		for (const auto& iter : cameFrom) {
			const auto& coords = iter.first;
			const int dist = Utils::distance(coords, goalCoords);
			if (dist < shortestDist) {
				shortestDist = dist;
				goalCoords = coords;
			}
		}
	}

	path.clear();
	path.push_back(goalCoords);
	auto nxt = cameFrom.at(goalCoords);
	int i = 0;
	while (nxt != std::nullopt) {
		path.push_back(*nxt);
		nxt = cameFrom.at(*nxt);
		i++;
	}
	const auto f = path.back();
	assert(f == myCoords);
}

void PathfindingComponent::draw() {
	if (!Globals::get().debug) {
		return;
	}
	/*for (int i = 0; i < map.navMap.size(); i++) {
		for (int j = 0; j < map.navMap[i].size(); j++) {
			auto rect = Globals::get().cameraRelative(map.getScaledTile(Vector2D<>(j, i)));
			const auto color = map.navMap[i][j] ? RGBVals::white() : RGBVals::black();
			Utils::drawRect(&rect, color);
		}
	}*/
	auto rect = Globals::get().cameraRelative(map.getScaledTile(goalCoords));
	Utils::drawRect(&rect, RGBVals::blue());
	for (const auto& coords : path) {
		auto rect = Globals::get().cameraRelative(map.getScaledTile(coords));
		Utils::drawRect(&rect, RGBVals::purple());
	}
}

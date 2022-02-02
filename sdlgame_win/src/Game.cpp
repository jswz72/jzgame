#include "Game.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <stdio.h>
#include <string>
#include <unordered_set>

#include "Collision.h"
#include "ECS/Components.h"
#include "ECS/ECS.h"
#include "KeyboardHandler.h"
#include "TextureManager.h"
#include "Vector2D.h"
#include "Globals.h"
#include "GroupLabel.h"

using std::cout;
using std::endl;

std::vector<SDL_Rect> testcols{};

EntityManager entityManager{ Globals::get().colliders };

const std::string terrainTexName = "terrain";
const std::string playerTexName = "player";
const std::string enemyTexName = "enemy";
const std::string projectileTexName = "projectile";
const std::unordered_map<std::string, std::string> textureFileNames = {
	{ terrainTexName, "terrain_ss.png" },
	{ playerTexName, "player_anims.png" },
	{ enemyTexName, "wizardidle.png" },
	{ projectileTexName, "proj.png" },
};

void Game::setCameraSize(int cameraW, int cameraH) {
	Globals::get().camera = { 0,0,cameraW,cameraH };
}

Game::Game(int ww, int wh, std::string title, bool fullscreen)
		: windowWidth(ww), windowHeight(wh), title(title), fullscreen(fullscreen) {
	assetPath = std::filesystem::current_path() / "assets";
	assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		windowWidth, windowHeight, flags);
	assert(window);
	Globals::get().renderer = SDL_CreateRenderer(window, -1, 0);
	assert(Globals::get().renderer);
	Utils::setRenderDrawColor(RGBVals::turquoise());
	Globals::get().isRunning = true;

	if (TTF_Init() == -1) {
		throw std::exception("Error in SDL_TTF");
	}

	loadAssets();
	// TODO camera doesn't work well for > 2 scale? Tie into this?
	const int mapScale = 2;
	const int tileSize = 32;
	const auto tilemapPath = assetPath / "mymap.map";
	const auto tilemapHeight = 20;
	const auto tilemapWidth = 25;
	const auto textureMappingPath = assetPath / "mymap.mappings";
	map = Map(tilemapPath, tilemapHeight, tilemapWidth, "terrain",
		textureMappingPath, tileSize, mapScale);
	initEntities();
	initUI();
	menu = std::make_unique<MenuSystem>(MenuSystem(windowWidth, windowHeight, window, Globals::get().renderer));

}

void Game::loadAssets() {
	auto &assetManager = Globals::get().assetManager;
	assetManager.addTexture(terrainTexName,
		assetPath / textureFileNames.at(terrainTexName));
	assetManager.addTexture(playerTexName,
		assetPath / textureFileNames.at(playerTexName));
	uint32_t enemyBackgroundColor[3] = { 58, 64, 65 };
	assetManager.addTexture(enemyTexName,
		assetPath / textureFileNames.at(enemyTexName), enemyBackgroundColor);
	assetManager.addTexture(projectileTexName,
		assetPath / textureFileNames.at(projectileTexName));
	const int fontSize = 16;
	assetManager.addFont("arial", assetPath / "arial.ttf", fontSize);
}

void Game::initPlayer() {
	Entity& player = entityManager.addEntity();

	// Roughly middle of screen.
	const Vector2D<> startingPos{ 750, 615 };
	const float pScale = 6;
	const float pSpeed = 2;
	const float hScale = 0.75f;
	const float wScale = 0.4f;
	const float xOffset = 0.3f;
	const float yOffset = 0.3f;
	auto& transformComp = player.addComponent<TransformComponent>(startingPos, pScale, pSpeed,
		hScale, wScale, xOffset, yOffset);
	const int srcH = 32, srcW = 32;
	auto& spriteComp = player.addComponent<SpriteComponent>(transformComp,
		playerTexName, srcH, srcW, true);
	player.addComponent<PlayerKeyboardController>(transformComp, spriteComp);
	player.addComponent<PlayerMouseController>();
	player.addComponent<ColliderComponent>(&transformComp);
	player.addComponent<HealthComponent>(100, &transformComp);
	player.addGroup(GroupLabel::Players);
}

void Game::initEnemies() {
	int numEnemies = 5;
	auto& tileEntities = entityManager.getGroup(GroupLabel::Map);
	std::vector<Entity*> spawnableTiles;
	for (auto const tileEntity : tileEntities) {
		if (tileEntity->getComponent<TileComponent>().getNavValue() > 0) {
			spawnableTiles.push_back(tileEntity);
		}
	}
	assert(!spawnableTiles.empty());
	std::vector<Entity*> toSpawn;
	std::sample(spawnableTiles.begin(), spawnableTiles.end(),
		std::back_inserter(toSpawn), numEnemies,
		std::mt19937{ std::random_device{}() });
	assert(!toSpawn.empty());
	for (int i = 0; i < numEnemies; i++) {
		auto& enemy = entityManager.addEntity();
		const auto& spawnTile = toSpawn[i]->getComponent<TileComponent>();
		const auto tileCenter = spawnTile.center();

		const float scale = 4;
		const float speed = 1;
		const float hScale = 1;
		const float wScale = 1;
		const float xOffset = 0;
		const float yOffset = 0;
		auto& transformComp = enemy.addComponent<TransformComponent>(tileCenter, scale, speed,
			hScale, wScale, xOffset, yOffset);
		int srcH = 80, srcW = 75;
		auto& spriteComp = enemy.addComponent<SpriteComponent>(transformComp, enemyTexName, srcH, srcW, false);
		enemy.addComponent<ColliderComponent>(&transformComp);
		enemy.addComponent<HealthComponent>(100, &transformComp);
		auto &transform = enemy.getComponent<TransformComponent>();
		enemy.addComponent<PathfindingComponent>( &transform, *map,
			transform.getPosition());
		enemy.addGroup(GroupLabel::Enemies);
	}
}

void Game::initEntities() {
	initPlayer();
	initEnemies();
}

void Game::initUI() {
	Entity& label = entityManager.addEntity();
	label.setTag("playerPosLabel");
	SDL_Color white = { 255, 255, 255, 255 };
	const bool debug = true;
	label.addComponent<UILabel>(10, 10, "", "arial", white, debug);
	label.addGroup(GroupLabel::UI);
}

void Game::createProjectile(Vector2D<> pos, Vector2D<> velocity, int range, float speed,
	std::string id, Entity* source) {
	auto& projectile = entityManager.addEntity();
	const float sizeX = 20, sizeY = 20;
	auto& transformComp = projectile.addComponent<TransformComponent>(pos, sizeX, sizeY, 1.f, speed);
	const int srcX = 32, srcY = 32;
	projectile.addComponent<SpriteComponent>(transformComp, id, srcX, srcY, false);
	projectile.addComponent<ColliderComponent>(&transformComp);
	int damage = 20;
	projectile.addComponent<ProjectileComponent>(transformComp, range, velocity, damage, source);
	projectile.addGroup(GroupLabel::Projectiles);
}

void projectileSubHealth(Entity* projectile, Entity* victim) {
	auto projSource = projectile->getComponent<ProjectileComponent>().source;
	if (projSource == victim) {
		return;
	}
	auto& victimHealth = victim->getComponent<HealthComponent>();
	victimHealth.healthSub(projectile->getComponent<ProjectileComponent>().damage);
}

void handleCollision(Entity* entityA, Entity* entityB, bool retry=true) {
	if (entityA->hasComponent<ProjectileComponent>() &&
		entityB->hasComponent<HealthComponent>()) {
		projectileSubHealth(entityA, entityB);
	}
	if (entityA->hasComponent<ProjectileComponent>() &&
		entityB->hasComponent<ColliderComponent>()) {
		if (entityA->getComponent<ProjectileComponent>().source != entityB) {
			entityA->destroy();
		}
	}
	if (retry) {
		handleCollision(entityB, entityA, false);
	}
}

void Game::handleCollisions() {
	const auto& mapBounds = map->getBounds();
	auto quadTree = QuadTree(0, SDL_Rect{ 0, 0, static_cast<int>(mapBounds.x),
		static_cast<int>(mapBounds.y) });
	for (auto& colliderComp : Globals::get().colliders) {
		quadTree.insert(colliderComp);
	}
	std::vector<ColliderComponent*> otherColliderComps;
	std::unordered_map<ColliderComponent*, std::unordered_set<ColliderComponent*>> handledCollisions;
	for (auto& colliderCompA : Globals::get().colliders) {
		otherColliderComps.clear();
		const auto& collA = colliderCompA->collider;
		quadTree.retrieve(otherColliderComps, collA);
		for (auto& colliderCompB : otherColliderComps) {
			if (colliderCompA == colliderCompB) {
				// Can't collide with self.
				continue;
			}
			auto collB = colliderCompB->collider;
			if (Collision::AABB(collA, collB)) {
				// Check if collision already handled.
				auto colASet = handledCollisions[colliderCompA];
				if (colASet.find(colliderCompB) != colASet.end()) {
					continue;
				}
				auto colBSet = handledCollisions[colliderCompB];
				if (colBSet.find(colliderCompA) != colBSet.end()) {
					continue;
				}
				handleCollision(colliderCompA->entity, colliderCompB->entity);
				handledCollisions[colliderCompA].insert(colliderCompB);
			}
		}
	}
}

void Game::updateCamera() {
	// TODO: swap out with get group?
	auto& players = entityManager.getGroup(GroupLabel::Players);
	assert(players.size() == 1);
	auto& player = players[0];
	Vector2D<> playerPos = player->getComponent<TransformComponent>().getPosition();
	auto& camera = Globals::get().camera;
	camera.x = static_cast<int>(playerPos.x) - (camera.w / 2);
	camera.y = static_cast<int>(playerPos.y) - (camera.h / 2);
	//Bounds.
	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;
}

void Game::setFpsString(int fps) {
	auto fpsLabel = entityManager.getEntityWithTag("fpsLabel");
	std::stringstream ss;
	ss << "FPS: " << fps;
	if (!fpsLabel) {
		Entity& label = entityManager.addEntity();
		label.setTag("fpsLabel");
		SDL_Color white = { 255, 255, 255, 255 };
		label.addComponent<UILabel>(windowWidth - 100, 10, ss.str(), "arial", white, true);
		label.addGroup(GroupLabel::UI);
		fpsLabel = entityManager.getEntityWithTag("fpsLabel");
	}
	assert(fpsLabel);
	fpsLabel->getComponent<UILabel>().setLabelText(ss.str(), "arial");
}

void drawQuadTree(QuadTree* quadTree) {
	if (!quadTree) return;
	testcols.push_back(static_cast<SDL_Rect>(Globals::get().cameraRelative(quadTree->bounds)));
	for (int i = 0; i < 4; i++) {
		drawQuadTree(quadTree->nodes[i]);
	}
}

bool Game::playerWillHitWall(const Rect &newPlayerRect, QuadTree &quadTree) { 
	std::vector<ColliderComponent*> otherColliderComps;
	quadTree.retrieve(otherColliderComps, newPlayerRect);
	testcols.clear();
	if (Globals::get().debug) {
		drawQuadTree(&quadTree);
	}
	for (auto& collider : otherColliderComps) {
		if (Globals::get().debug) {
			testcols.push_back(static_cast<SDL_Rect>(Globals::get().cameraRelative(collider->collider)));
		}
		if (Collision::AABB(collider->collider, newPlayerRect)) {
			return true;
		}
	}
	return false;
}

Vector2D<> Game::checkPlayerMovement(Entity* player) {
	const auto& transform = player->getComponent<TransformComponent>();
	const auto& collider = player->getComponent<ColliderComponent>();
	const auto oldPos = transform.getPosition();

	const auto mapBounds = map->getBounds();
	auto quadTree = QuadTree(0, SDL_Rect{ 0, 0, static_cast<int>(mapBounds.x),
		static_cast<int>(mapBounds.y) });
	for (auto& colliderEntity : entityManager.getGroup(GroupLabel::Colliders)) {
		quadTree.insert(&colliderEntity->getComponent<ColliderComponent>());
	}

	const auto newCol = [&](const Vector2D<> &newPos) {
		auto newCol = collider.collider;
		newCol.x = newPos.x;
		newCol.y = newPos.y;
		return newCol;
	};

	auto nextPos = transform.getNewPosition();
	auto col = newCol(nextPos);
	if (!playerWillHitWall(col, quadTree)) {
		return nextPos;
	}
	// Try with only Y delta.
	nextPos.x = oldPos.x;
	if (!playerWillHitWall(newCol(nextPos), quadTree)) {
		return nextPos;
	}
	// Try with only X delta.
	nextPos = transform.getNewPosition();
	nextPos.y = oldPos.y;
	if (!playerWillHitWall(newCol(nextPos), quadTree)) {
		return nextPos;
	}

	return oldPos;
}

void Game::update() {
	if (Globals::get().isPaused) {
		return;
	}
	uint32_t currTime = SDL_GetTicks();
	Globals::get().timeDelta = (currTime - lastTicks) / 10.0f;
	lastTicks = currTime;
	
	auto& players = entityManager.getGroup(GroupLabel::Players);
	assert(players.size() == 1);
	auto player = players[0];
	auto& playerTransComp = player->getComponent<TransformComponent>();
	Vector2D<> oldPlayerPos = playerTransComp.getPosition();
	const auto& playerCollider = player->getComponent<ColliderComponent>().collider;
	
	std::stringstream ss;
	ss << "Player transform position: " << playerTransComp.getPosition().x << ","
		<< playerTransComp.getPosition().y;
	ss << "Player collider position: " << playerCollider.x << "," << playerCollider.y;
	auto playerPosLabel = entityManager.getEntityWithTag("playerPosLabel");
	assert(playerPosLabel);
	playerPosLabel->getComponent<UILabel>().setLabelText(ss.str(), "arial");

	// Apply player movement and preempt any collisions. TODO: use this pattern for all
	// entities with colliders, instead of applying all movements then resolving collisions
	// afterwards.
	const auto newPlayerPos = checkPlayerMovement(player);
	// Setting new position now so that enemies can pathfind to it.
	playerTransComp.setPosition(newPlayerPos);
	for (const auto& enemy : entityManager.getGroup(GroupLabel::Enemies)) {
		enemy->getComponent<PathfindingComponent>().setGoal(playerTransComp.getCenter());
	}
	entityManager.refresh();
	entityManager.update();
	// Reset our resolved position that update may have overwritten. TODO: prevent it from getting overwritten.
	playerTransComp.setPosition(newPlayerPos);

	handleCollisions();
	updateCamera();
}

void Game::render() {
	SDL_RenderClear(Globals::get().renderer);
	

	// Render groups one at a time.

	auto& tileEntities(entityManager.getGroup(GroupLabel::Map));
	for (auto& tile : tileEntities) {
		tile->draw();
	}

	auto& colliderEntities(entityManager.getGroup(GroupLabel::Colliders));
	for (auto& collider : colliderEntities) {
		collider->draw();
	}

	auto& playerEntities(entityManager.getGroup(GroupLabel::Players));
	for (auto& player : playerEntities) {
		player->draw();
	}

	auto& enemyEntities(entityManager.getGroup(GroupLabel::Enemies));
	for (auto& enemy : enemyEntities) {
		enemy->draw();
	}

	auto& projectileEntities(entityManager.getGroup(GroupLabel::Projectiles));
	for (auto& projectile : projectileEntities) {
		projectile->draw();
	}
	// Draw UI over game state.
	auto& uiEntities(entityManager.getGroup(GroupLabel::UI));
	for (auto& ui : uiEntities) {
		ui->draw();
	}
	for (auto &testcol : testcols) {
		Utils::drawRect(&testcol, RGBVals::green());
	}

	// Draw menu over everything.
	menu->draw();

	SDL_RenderPresent(Globals::get().renderer);
}

void Game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			Globals::get().isRunning = false;
		}
		if (Globals::get().isPaused) {
			menu->handleEvents(&event);
		}
		Globals::get().keyboardHandler.handleKeyboardEvent(event.key);
		Globals::get().mouseButtonHandler.handleMouseButtonEvent(event.button);
	}
}

Game::~Game() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(Globals::get().renderer);
	SDL_Quit();
}
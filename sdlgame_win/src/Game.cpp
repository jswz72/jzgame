#include <algorithm>
#include <cassert>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <random>
#include <sstream>
#include <stdio.h>
#include <string>
#include <unordered_set>
#include "AssetManager.h"
#include "Collision.h"
#include "ECS/Components.h"
#include "ECS/ECS.h"
#include "Game.h"
#include "KeyboardHandler.h"
#include "Map.h"
#include "TextureManager.h"
#include "Vector2D.h"

using std::cout;
using std::endl;
using std::string;

bool Game::isRunning = false;
bool Game::isPaused = false;
bool Game::debug = true;
SDL_Renderer* Game::renderer = nullptr;
SDL_Rect Game::camera = { 0,0,0,0 };
std::vector<ColliderComponent*> Game::colliders;
AssetManager Game::assetManager = AssetManager();
float Game::timeDelta = 0;
KeyboardHandler Game::keyboardHandler{};
MouseButtonHandler Game::mouseButtonHandler{};
Vector2D Game::mapBounds = Vector2D{};
std::vector<SDL_Rect> Game::testcols{};

EntityManager entityManager{ Game::colliders };

void Game::setCameraSize(int cameraW, int cameraH) {
	camera = { 0,0,cameraW,cameraH };
}

Game::Game(int ww, int wh) : windowWidth(ww), windowHeight(wh) {
	assetPath = std::filesystem::current_path() / "assets";
}

void Game::loadAssets() {
	assetManager.addTexture("terrain", assetPath / "terrain_ss.png");
	assetManager.addTexture("player", assetPath / "player_anims.png");
	uint32_t enemyBackgroundColor[3] = { 58, 64, 65 };
	assetManager.addTexture("enemy", assetPath / "wizardidle.png", enemyBackgroundColor);
	assetManager.addTexture("projectile", assetPath / "proj.png");
	int fontSize = 16;
	assetManager.addFont("arial", assetPath / "arial.ttf", fontSize);
}

void Game::initPlayer() {
	// Roughly middle of screen.
	Entity& player = entityManager.addEntity();
	player.setTag("player");
	Vector2D startingPos{ 750, 615 };
	const int pScale = 6;
	const float pSpeed = 2;
	const float hScale = 0.75f;
	const float wScale = 0.4f;
	const float xOffset = 0.3f;
	const float yOffset = 0.3f;
	auto& transformComp = player.addComponent<TransformComponent>(startingPos, pScale, pSpeed,
		hScale, wScale, xOffset, yOffset);
	int srcH = 32, srcW = 32;
	auto& spriteComp = player.addComponent<SpriteComponent>(transformComp, "player", srcH, srcW, true);
	player.addComponent<PlayerKeyboardController>(&transformComp, &spriteComp);
	player.addComponent<PlayerMouseController>();
	player.addComponent<ColliderComponent>("player", &transformComp);
	player.addComponent<HealthComponent>(100, &transformComp);
	player.addGroup(groupPlayers);
}

void Game::initEnemies() {
	int numEnemies = 10;
	auto& tileEntities = entityManager.getGroup(Game::groupMap);
	std::vector<Entity*> spawnableTiles;
	for (auto const tileEntity : tileEntities) {
		if (tileEntity->getTag() == "tileland") {
			spawnableTiles.push_back(tileEntity);
		}
	}
	assert(spawnableTiles.size() > 0);
	assert(spawnableTiles.size() > numEnemies);
	std::vector<Entity*> toSpawn;
	std::sample(spawnableTiles.begin(), spawnableTiles.end(),
		std::back_inserter(toSpawn), numEnemies,
		std::mt19937{ std::random_device{}() });
	assert(toSpawn.size() > 0);
	for (int i = 0; i < numEnemies; i++) {
		auto& enemy = entityManager.addEntity();
		enemy.setTag("enemy");
		const auto& spawnTile = toSpawn[i]->getComponent<TileComponent>();
		auto tileCenter = Vector2D{ static_cast<float>(spawnTile.position.x + 0.5 * spawnTile.tileSize),
			static_cast<float>(spawnTile.position.y + 0.5 * spawnTile.tileSize) };
		const int scale = 4;
		const float speed = 1;
		const float hScale = 1;
		const float wScale = 1;
		const float xOffset = 0;
		const float yOffset = 0;
		auto& transformComp = enemy.addComponent<TransformComponent>(tileCenter, scale, speed,
			hScale, wScale, xOffset, yOffset);
		int srcH = 80, srcW = 75;
		auto& spriteComp = enemy.addComponent<SpriteComponent>(transformComp, "enemy", srcH, srcW, false);
		enemy.addComponent<ColliderComponent>("enemy", &transformComp);
		enemy.addComponent<HealthComponent>(100, &transformComp);
		enemy.addGroup(groupEnemies);
	}
}

void Game::initEntities() {
	initPlayer();
	initEnemies();
}

void Game::initUI() {
	Entity& label = entityManager.addEntity();
	label.setTag("label");
	SDL_Color white = { 255, 255, 255, 255 };
	const bool debug = true;
	label.addComponent<UILabel>(10, 10, "Test string", "arial", white, debug);
	label.addGroup(groupUI);
}

void Game::init(char const* title, bool fullscreen) {
	assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);
	cout << "Subsystems initialized..." << endl;
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		windowWidth, windowHeight, flags);
	assert(window);
	cout << "Window created" << endl;
	renderer = SDL_CreateRenderer(window, -1, 0);
	assert(renderer);
	cout << "Renderer created" << endl;
	SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
	isRunning = true;

	if (TTF_Init() == -1) {
		throw std::exception("Error in SDL_TTF");
	}

	loadAssets();
	// TODO camera doesn't work well for > 2 scale? Tie into this?
	Map* map = new Map("terrain", 2, 32);

	navMap = map->loadMap(assetPath / "mymap.map", assetPath / "mymap.mappings", 25, 20);
	mapBounds = map->bounds();
	initEntities();
	initUI();
	menu = new MenuSystem(windowWidth, windowHeight, window, renderer);
}

void Game::createProjectile(Vector2D pos, Vector2D velocity, int range, float speed,
	std::string id, Entity* source) {
	auto& projectile = entityManager.addEntity();
	projectile.setTag("projectile");
	const int sizeX = 20, sizeY = 20;
	auto& transformComp = projectile.addComponent<TransformComponent>(pos, sizeX, sizeY, 1, speed);
	const int srcX = 32, srcY = 32;
	projectile.addComponent<SpriteComponent>(transformComp, id, srcX, srcY, false);
	projectile.addComponent<ColliderComponent>("projectile", &transformComp);
	int damage = 20;
	projectile.addComponent<ProjectileComponent>(transformComp, range, velocity, damage, source);
	projectile.addGroup(Game::groupProjectiles);
}

void handlePlayerHitWall(Entity* player, Entity* wall) {
	// TODO
}

void handleProjectileHitPlayer(Entity* projectile, Entity* player) {
	auto projSource = projectile->getComponent<ProjectileComponent>().source;
	auto playerCol = player->getComponent<ColliderComponent>().collider;
	auto projectileCol = projectile->getComponent<ColliderComponent>().collider;
	if (projSource != player) {
		std::cout << "Projectile hit player" << std::endl;
		projectile->destroy();
	}
}

void handleProjectileHitEnemy(Entity* projectile, Entity* enemy) {
	auto& enemyHealth = enemy->getComponent<HealthComponent>();
	enemyHealth.healthSub(projectile->getComponent<ProjectileComponent>().damage);
	projectile->destroy();
}

void handleCollision(Entity* entityA, Entity* entityB, Vector2D prevPlayerPos) {
	auto tagA = entityA->getTag();
	auto tagB = entityB->getTag();
	if (tagA == "projectile" && tagB == "tileCollider") {
		entityA->destroy();
	}
	else if (tagB == "projectile" && tagA == "tileCollider") {
		entityB->destroy();
	}
	else if (tagA == "player" && tagB == "tileCollider") {
		handlePlayerHitWall(entityA, entityB);
	}
	else if (tagB == "player" && tagA == "tileCollider") {
		handlePlayerHitWall(entityB, entityA);
	}
	else if (tagA == "player" && entityB->hasComponent<ProjectileComponent>()) {
		handleProjectileHitPlayer(entityB, entityA);
	}
	else if (tagB == "player" && entityA->hasComponent<ProjectileComponent>()) {
		handleProjectileHitPlayer(entityA, entityB);
	}
	// TODO, sometimes get same collision for both of these.
	else if (tagA == "enemy" && entityB->hasComponent<ProjectileComponent>()) {
		handleProjectileHitEnemy(entityB, entityA);
	}
	else if (tagB == "enemy" && entityA->hasComponent<ProjectileComponent>()) {
		handleProjectileHitEnemy(entityA, entityB);
	}
}

void Game::handleCollisions(Vector2D prevPlayerPos) {
	auto quadTree = QuadTree(0, SDL_Rect{ 0, 0, static_cast<int>(mapBounds.x),
		static_cast<int>(mapBounds.y) });
	for (auto& colliderComp : colliders) {
		quadTree.insert(colliderComp);
	}
	std::vector<ColliderComponent*> otherColliderComps;
	std::unordered_map<ColliderComponent*, std::unordered_set<ColliderComponent*>> handledCollisions;
	for (auto& colliderCompA : colliders) {
		otherColliderComps.clear();
		auto collA = colliderCompA->collider;
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
				handleCollision(colliderCompA->entity, colliderCompB->entity,
					prevPlayerPos);
				handledCollisions[colliderCompA].insert(colliderCompB);
			}
		}
	}
}

void Game::updateCamera() {
	auto player = entityManager.getEntityWithTag("player");
	Vector2D playerPos = player->getComponent<TransformComponent>().getPosition();
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
		label.addGroup(groupUI);
		fpsLabel = entityManager.getEntityWithTag("fpsLabel");
	}
	assert(fpsLabel);
	fpsLabel->getComponent<UILabel>().setLabelText(ss.str(), "arial");
}

void drawQuadTree(QuadTree* quadTree) {
	if (!quadTree) return;
	auto testcol = quadTree->bounds;
	testcol.x -= Game::camera.x;
	testcol.y -= Game::camera.y;
	Game::testcols.push_back(testcol);
	for (int i = 0; i < 4; i++) {
		drawQuadTree(quadTree->nodes[i]);
	}
}

bool Game::playerWillHitWall(SDL_Rect newPlayerRect) { 
	auto quadTree = QuadTree(0, SDL_Rect{ 0, 0, static_cast<int>(mapBounds.x),
		static_cast<int>(mapBounds.y) });
	for (auto& colliderComp : colliders) {
		if (colliderComp->tag == "terrain") {
			quadTree.insert(colliderComp);
		}
	}
	std::vector<ColliderComponent*> otherColliderComps;
	quadTree.retrieve(otherColliderComps, newPlayerRect);
	testcols.clear();
	drawQuadTree(&quadTree);
	// TODO quadtree seems to be putting some colliders in incorrect spots
	for (auto& collider : otherColliderComps) {
		auto testcol = collider->collider;
		testcol.x -= Game::camera.x;
		testcol.y -= Game::camera.y;
		testcols.push_back(testcol);
		if (Collision::AABB(collider->collider, newPlayerRect)) {
			return true;
		}
	}
	return false;
}

Vector2D Game::checkPlayerMovement(Entity* player) {
	const auto& transform = player->getComponent<TransformComponent>();
	const auto& collider = player->getComponent<ColliderComponent>();
	const auto oldPos = transform.getPosition();

	const auto newCol = [&](const Vector2D &newPos) {
		auto newCol = collider.collider;
		newCol.x = newPos.x;
		newCol.y = newPos.y;
		return newCol;
	};

	auto nextPos = transform.getNewPosition();
	auto col = newCol(nextPos);
	if (!playerWillHitWall(col)) {
		return nextPos;
	}
	// Try with only Y delta.
	nextPos.x = oldPos.x;
	if (!playerWillHitWall(newCol(nextPos))) {
		return nextPos;
	}
	// Try with only X delta.
	nextPos = transform.getNewPosition();
	nextPos.y = oldPos.y;
	if (!playerWillHitWall(newCol(nextPos))) {
		return nextPos;
	}

	return oldPos;
}

void Game::update() {
	if (isPaused) {
		return;
	}
	int currTime = SDL_GetTicks();
	timeDelta = (currTime - lastTicks) / 10.0f;
	lastTicks = currTime;
	auto player = entityManager.getEntityWithTag("player");
	auto& playerTrans = player->getComponent<TransformComponent>();
	Vector2D oldPlayerPos = playerTrans.getPosition();
	auto playerCollider = player->getComponent<ColliderComponent>().collider;
	std::stringstream ss;
	ss << "Transform position: " << playerTrans.getPosition().x << "," << playerTrans.getPosition().y;
	ss << "Collider position: " << playerCollider.x << "," << playerCollider.y;
	auto label = entityManager.getEntityWithTag("label");
	label->getComponent<UILabel>().setLabelText(ss.str(), "arial");

	const auto newPlayerPos = checkPlayerMovement(player);
	entityManager.refresh();
	entityManager.update();
	playerTrans.setPosition(newPlayerPos);

	handleCollisions(oldPlayerPos);
	updateCamera();
}

void Game::render() {
	SDL_RenderClear(renderer);
	

	// Render groups one at a time.

	auto& tileEntities(entityManager.getGroup(Game::groupMap));
	for (auto& tile : tileEntities) {
		tile->draw();
	}

	auto& colliderEntities(entityManager.getGroup(Game::groupColliders));
	for (auto& collider : colliderEntities) {
		collider->draw();
	}

	auto& playerEntities(entityManager.getGroup(Game::groupPlayers));
	for (auto& player : playerEntities) {
		player->draw();
	}

	auto& enemyEntities(entityManager.getGroup(Game::groupEnemies));
	for (auto& enemy : enemyEntities) {
		enemy->draw();
	}

	auto& projectileEntities(entityManager.getGroup(Game::groupProjectiles));
	for (auto& projectile : projectileEntities) {
		projectile->draw();
	}
	// Draw UI over game state. TODO maybe replace with KISS UI.
	auto& uiEntities(entityManager.getGroup(Game::groupUI));
	for (auto& ui : uiEntities) {
		ui->draw();
	}
	for (auto &testcol : testcols) {
		SDL_SetRenderDrawColor(Game::renderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(Game::renderer, &testcol);
	}

	// Draw menu over everything.
	menu->draw();

	SDL_RenderPresent(renderer);
}

void Game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			isRunning = false;
		}
		if (isPaused) {
			menu->handleEvents(&event);
		}
		keyboardHandler.handleKeyboardEvent(event.key);
		mouseButtonHandler.handleMouseButtonEvent(event.button);
	}
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	cout << "Game cleaned" << endl;
}
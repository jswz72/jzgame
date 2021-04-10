#include <cassert>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
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

Manager manager;

bool Game::isRunning = false;
bool Game::isPaused = false;
bool Game::debug = true;
SDL_Renderer* Game::renderer = nullptr;
SDL_Rect Game::camera = { 0,0,0,0 };
AssetManager* Game::assets = new AssetManager(&manager);
std::vector<ColliderComponent*> Game::colliders;
float Game::timeDelta = 0;
KeyboardHandler Game::keyboardHandler{};
MouseButtonHandler Game::mouseButtonHandler{};

void Game::setCameraSize(int cameraW, int cameraH) {
	camera = { 0,0,cameraW,cameraH };
}

Game::Game(int ww, int wh) : windowWidth(ww), windowHeight(wh) {
	assetPath = std::filesystem::current_path() / "assets";
}

Map *map = new Map("terrain", 2, 32);
void Game::loadAssets() {
	assets->addTexture("terrain", assetPath / "terrain_ss.png");
	assets->addTexture("player", assetPath / "player_anims.png");
	assets->addTexture("projectile", assetPath / "proj.png");
	int fontSize = 16;
	assets->addFont("arial", assetPath / "arial.ttf", fontSize);
	map->loadMap(assetPath / "mymap.map", 25, 20);
}

void Game::initPlayer() {
	// Roughly middle of screen.
	Entity& player = manager.addEntity();
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
	player.addGroup(groupPlayers);
}

void Game::initEntities() {
	initPlayer();
}

void Game::initUI() {
	Entity& label = manager.addEntity();
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
	quadTree = new QuadTree(0, SDL_Rect{ 0, 0, map->boundsX, map->boundsY });
	initEntities();
	initUI();
	menu = new MenuSystem(windowWidth, windowHeight, window, renderer);
}

void Game::createProjectile(Vector2D pos, Vector2D velocity, int range, float speed,
	std::string id, Entity* source) {
	auto& projectile = manager.addEntity();
    projectile.setTag("projectile");
    const int sizeX = 20, sizeY = 20;
    auto& transformComp = projectile.addComponent<TransformComponent>(pos, sizeX, sizeY, 1, speed);
    const int srcX = 32, srcY = 32;
    projectile.addComponent<SpriteComponent>(transformComp, id, srcX, srcY, false);
    projectile.addComponent<ColliderComponent>("projectile", &transformComp);
    projectile.addComponent<ProjectileComponent>(transformComp, range, velocity, source);
    projectile.addGroup(Game::groupProjectiles);
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

void handleCollision(Entity* entityA, Entity* entityB, Vector2D prevPlayerPos) {
	auto tagA = entityA->getTag();
	auto tagB = entityB->getTag();
	if (tagA == "projectile" && tagB == "tileCollider") {
		entityA->destroy();
	} else if (tagB == "projectile" && tagA == "tileCollider") {
		entityB->destroy();
	} else if (tagA == "player" && tagB == "tileCollider") {
		entityA->getComponent<TransformComponent>().setPosition(prevPlayerPos);
	} else if (tagB == "player" && tagA == "tileCollider") {
		entityB->getComponent<TransformComponent>().setPosition(prevPlayerPos);
	} else if (tagA == "player" && entityB->hasComponent<ProjectileComponent>()) {
		handleProjectileHitPlayer(entityB, entityA);
	} else if (tagB == "player" && entityA->hasComponent<ProjectileComponent>()) {
		handleProjectileHitPlayer(entityA, entityB);
	}
}

void Game::handleCollisions(Vector2D prevPlayerPos) {
	quadTree->clear();
	for (auto &entity : manager.entities) {
		if (entity->hasComponent<ColliderComponent>()) {
			quadTree->insert(entity.get());
		}
	}
	std::vector<Entity*> collEntities;
	for (auto& entity : manager.entities) {
		collEntities.clear();
		if (entity->hasComponent<ColliderComponent>()) {
			auto collA = entity->getComponent<ColliderComponent>().collider;
			quadTree->retrieve(collEntities, collA);
			for (auto& collEntity : collEntities) {
				if (collEntity == entity.get()) {
					continue;
				}
				auto collB = collEntity->getComponent<ColliderComponent>().collider;
				if (Collision::AABB(collA, collB)) {
					handleCollision(entity.get(), collEntity, prevPlayerPos);
				}
			}
		}
	}
}

void Game::updateCamera() {
	auto player = manager.getEntityWithTag("player");
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
	auto fpsLabel = manager.getEntityWithTag("fpsLabel");
	std::stringstream ss;
	ss << "FPS: " << fps;
	if (!fpsLabel) {
		Entity& label = manager.addEntity();
		label.setTag("fpsLabel");
		SDL_Color white = { 255, 255, 255, 255 };
		label.addComponent<UILabel>(windowWidth - 100, 10, ss.str(), "arial", white, true);
		label.addGroup(groupUI);
		fpsLabel = manager.getEntityWithTag("fpsLabel");
	}
	assert(fpsLabel);
	fpsLabel->getComponent<UILabel>().setLabelText(ss.str(), "arial");
}

void Game::update() {
	if (isPaused) {
		return;
	}
	int currTime = SDL_GetTicks();
	timeDelta = (currTime - lastTicks) / 10.0f;
	lastTicks = currTime;
	auto player = manager.getEntityWithTag("player");
	auto playerTrans = player->getComponent<TransformComponent>();
	Vector2D playerPos = playerTrans.getPosition();
	auto playerCollider = player->getComponent<ColliderComponent>().collider;
	std::stringstream ss;
	ss << "Transform position: " << playerTrans.getPosition().x << "," << playerTrans.getPosition().y;
	ss << "Collider position: " << playerCollider.x << "," << playerCollider.y;
	auto label = manager.getEntityWithTag("label");
	label->getComponent<UILabel>().setLabelText(ss.str(), "arial");

	manager.refresh();
	manager.update();

	handleCollisions(playerPos);
	updateCamera();
}

void Game::render() {
	SDL_RenderClear(renderer);
	// Render groups one at a time.

	auto& tileEntities(manager.getGroup(Game::groupMap));
	for (auto& tile : tileEntities) {
		tile->draw();
	}

	auto& colliderEntities(manager.getGroup(Game::groupColliders));
	for (auto& collider : colliderEntities) {
		collider->draw();
	}

	auto& playerEntities(manager.getGroup(Game::groupPlayers));
	for (auto& player : playerEntities) {
		player->draw();
	}
	
	auto& projectileEntities(manager.getGroup(Game::groupProjectiles));
	for (auto& projectile : projectileEntities) {
		projectile->draw();
	}
	// Draw UI over game state. TODO maybe replace with KISS UI.
	auto& uiEntities(manager.getGroup(Game::groupUI));
	for (auto& ui : uiEntities) {
		ui->draw();
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

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
SDL_Renderer* Game::renderer = nullptr;
// TODO connect to window size
SDL_Rect Game::camera = { 0,0,800,600 };
AssetManager* Game::assets = new AssetManager(&manager);
std::vector<ColliderComponent*> Game::colliders;
float Game::timeDelta = 0;
KeyboardHandler Game::keyboardHandler{};
MouseButtonHandler Game::mouseButtonHandler{};

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

void Game::loadEntities() {
	// Roughly middle of screen.
	Entity& player = manager.addEntity();
	player.setTag("player");
	Vector2D startingPos{ 750, 615 };
	const int pScale = 6;
	const float pSpeed = 2;
	player.addComponent<TransformComponent>(startingPos, pScale, pSpeed);
	int srcX = 32, srcY = 32;
	player.addComponent<SpriteComponent>("player", srcX, srcY, true);
	player.addComponent<KeyboardController>();
	player.addComponent<MouseController>();
	const float colliderxOffset = 0.3f;
	const float collideryOffset = 0.3f;
	const float colliderwScale = 0.4f;
	const float colliderhScale = 0.75f;
	player.addComponent<ColliderComponent>("player", colliderxOffset, collideryOffset,
		colliderwScale, colliderhScale);
	player.addGroup(groupPlayers);
}

void Game::loadUI() {
	Entity& label = manager.addEntity();
	label.setTag("label");
	SDL_Color white = { 255, 255, 255, 255 };
	label.addComponent<UILabel>(10, 10, "Test string", "arial", white);
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
	loadEntities();
	loadUI();
	//assets->createProjectile(Vector2D(552, 594), Vector2D(2, 0), 1000, 2, "projectile");
	//assets->createProjectile(Vector2D(552, 594), Vector2D(1, 0), 1000, 2, "projectile");
	//assets->createProjectile(Vector2D(552, 594), Vector2D(0.5, 0.75), 1000, 2, "projectile");
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
		entityA->getComponent<TransformComponent>().position = prevPlayerPos;
	} else if (tagB == "player" && tagA == "tileCollider") {
		entityB->getComponent<TransformComponent>().position = prevPlayerPos;
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
	Vector2D playerPos = player->getComponent<TransformComponent>().position;
	camera.x = static_cast<int>(playerPos.x) - (windowWidth / 2);
	camera.y = static_cast<int>(playerPos.y) - (windowHeight / 2);
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

void Game::update() {
	int currTime = SDL_GetTicks();
	timeDelta = (currTime - lastTicks) / 10.0f;
	lastTicks = currTime;
	auto player = manager.getEntityWithTag("player");
	auto playerTrans = player->getComponent<TransformComponent>();
	Vector2D playerPos = playerTrans.position;
	auto playerCollider = player->getComponent<ColliderComponent>().collider;
	std::stringstream ss;
	ss << "Player hw: (" << playerTrans.getHeight() << ", " << playerTrans.getWidth() << " )" << std::endl;
	ss << "Player collider position: " << playerCollider.x << "," << playerCollider.y;
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
	// Draw UI over everything.
	auto& uiEntities(manager.getGroup(Game::groupUI));
	for (auto& ui : uiEntities) {
		ui->draw();
	}

	SDL_RenderPresent(renderer);
}

void Game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			keyboardHandler.handleKeyboardEvent(event.key);
			mouseButtonHandler.handleMouseButtonEvent(event.button);
		}
	}
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	cout << "Game cleaned" << endl;
}

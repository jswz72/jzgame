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
SDL_Rect Game::camera = { 0,0,800,640 };
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
	const auto pScale = 4;
	player.addComponent<TransformComponent>(startingPos, pScale, 2);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<MouseController>();
	player.addComponent<ColliderComponent>("player", 40, 35, 0.4, 0.75);
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
	loadEntities();
	loadUI();
	assets->createProjectile(Vector2D(552, 594), Vector2D(2, 0), 200, 2, "projectile");
}

void Game::handleCollisions(Vector2D prevPlayerPos) {
	auto player = manager.getEntityWithTag("player");
	SDL_Rect playerCol = player->getComponent<ColliderComponent>().collider;
	auto& colliderEntities(manager.getGroup(Game::groupColliders));
	for (auto& ce : colliderEntities) {
		SDL_Rect cCol = ce->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol)) {
			player->getComponent<TransformComponent>().position = prevPlayerPos;
		}
	}

	auto& projectileEntities(manager.getGroup(Game::groupProjectiles));
	for (auto& projectile : projectileEntities) {
		auto playerCol = player->getComponent<ColliderComponent>().collider;
		auto projectileCol = projectile->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(projectileCol, playerCol)) {
			std::cout << "Projectile hit player" << std::endl;
			projectile->destroy();
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
	Vector2D playerPos = player->getComponent<TransformComponent>().position;
	auto playerCollider = player->getComponent<ColliderComponent>().collider;
	std::stringstream ss;
	ss << "Player position: " << playerPos;
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

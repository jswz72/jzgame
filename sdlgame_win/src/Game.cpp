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
#include "Map.h"
#include "TextureManager.h"
#include "Vector2D.h"

using std::cout;
using std::endl;
using std::string;

bool DEBUG = true;

#define debug(a) if (DEBUG) std::cout << a << std::endl;

Map* map;
Manager manager;

const int GAME_MAP_WIDTH = 800;
const int GAME_MAP_HEIGHT = 640;
SDL_Rect Game::camera = { 0,0,800,640 };

AssetManager* Game::assets = new AssetManager(&manager);

std::vector<ColliderComponent*> Game::colliders;

bool Game::isRunning = false;

Entity& player = manager.addEntity();
Entity& label = manager.addEntity();
SDL_Event Game::event;

SDL_Renderer* Game::renderer = nullptr;

Game::Game() {}

Game::~Game() {}

void Game::init(char const* title, int width, int height, bool fullscreen) {
	assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);
	debug("Subsystems initialized...");
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, flags);
	assert(window);
	debug("Window created");
	renderer = SDL_CreateRenderer(window, -1, 0);
	assert(renderer);
	debug("Renderer created");
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	isRunning = true;

	if (TTF_Init() == -1) {
		throw std::exception("Error in SDL_TTF");
	}

	auto assetPath = std::filesystem::current_path() / "assets";
	assets->addTexture("terrain", assetPath / "terrain_ss.png");
	assets->addTexture("player", assetPath / "player_anims.png");
	assets->addTexture("projectile", assetPath / "proj.png");

	int fontSize = 16;
	assets->addFont("arial", assetPath / "arial.ttf", fontSize);

	map = new Map("terrain", 2, 32);
	map->loadMap(assetPath / "mymap.map", 25, 20);

	// Entities

   // Roughly middle of screen.
	Vector2D startingPos{ 750, 615 };
	// const auto pHeight = 58;
	// const auto pWidth = 32;
	const auto pScale = 4;
	// player.addComponent<TransformComponent>(startingPos, 80, 80, pScale);
	player.addComponent<TransformComponent>(startingPos, pScale);
	// uint32_t playerBackgroundColor[3] = {58, 64, 65};
   // const auto spriteSrcX = 21;
   // const auto spriteSrcY = 12;
	player.addComponent<SpriteComponent>("player", true);
	// playerBackgroundColor);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);

	SDL_Color white = { 255, 255, 255, 255 };
	label.addComponent<UILabel>(10, 10, "Test string", "arial", white);

	assets->createProjectile(Vector2D(552, 594), Vector2D(2, 0), 200, 2, "projectile");
	// assets->createProjectile(Vector2D(600, 620), Vector2D(2, 0), 200, 2, "projectile");
	// assets->createProjectile(Vector2D(400, 600), Vector2D(2, 1), 200, 2, "projectile");
	// assets->createProjectile(Vector2D(600, 600), Vector2D(2, -1), 200, 2, "projectile");
}


auto& tileEntities(manager.getGroup(Game::groupMap));
auto& playerEntities(manager.getGroup(Game::groupPlayers));
// Don't think groupColliders is added to anywhere??
auto& colliderEntities(manager.getGroup(Game::groupColliders));
auto& projectileEntities(manager.getGroup(Game::groupProjectiles));


void Game::update() {

	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;
	std::stringstream ss;
	ss << "Player position: " << playerPos;

	label.getComponent<UILabel>().setLabelText(ss.str(), "arial");

	manager.refresh();
	manager.update();

	for (auto& c : colliderEntities) {
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol)) {
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}

	for (auto& projectile : projectileEntities) {
		auto playerCol = player.getComponent<ColliderComponent>().collider;
		auto projectileCol = projectile->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(projectileCol, playerCol)) {
			std::cout << "Projectile hit player" << std::endl;
			projectile->destroy();
		}
	}
	std::cout << player.getComponent<TransformComponent>().position << std::endl;

	camera.x = static_cast<int>(player.getComponent<TransformComponent>().position.x) -
		(GAME_MAP_WIDTH / 2);
	camera.y = static_cast<int>(player.getComponent<TransformComponent>().position.y) -
		(GAME_MAP_HEIGHT / 2);

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

void Game::render() {
	SDL_RenderClear(renderer);
	// Render groups one at a time.
	for (auto& tile : tileEntities) {
		tile->draw();
	}

	for (auto& collider : colliderEntities) {
		collider->draw();
	}

	for (auto& player : playerEntities) {
		player->draw();
	}
	for (auto& projectile : projectileEntities) {
		projectile->draw();
	}
	// Draw UI over everything.
	label.draw();


	SDL_RenderPresent(renderer);
}

void Game::handleEvents() {
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	cout << "Game cleaned" << endl;
}

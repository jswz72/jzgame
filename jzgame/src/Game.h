#ifndef GAME
#define GAME
#include <filesystem>
#include <queue>
#include <vector>
#include "SDL.h"
#include "SDL_Image.h"
#include "AssetManager.h"
#include "EntityComponent.h"
#include "KeyboardHandler.h"
#include "MenuSystem.h"
#include "MouseHandler.h"
#include "QuadTree.h"
#include "Vector2D.h"
#include "Map.h"
#include "Globals.h"

class ColliderComponent;
class AssetManager;

class Game {
public:
	std::string title = nullptr;
	bool fullscreen = false;

	Game(int windowWidth, int windowHeight, std::string title, bool fullscreen);
	~Game();

	void loadAssets();
	void initPlayer();
	void initEnemies();
	void initEntities();
	void initUI();
	void handleCollisions();
	void updateCamera();
	void update();
	void render();
	void handleEvents();
	bool running() { return Globals::get().isRunning; }
	void setFpsString(int fps);
	bool playerWillHitWall(const Rect &newPlayerRect, QuadTree &quadTree);
	Vector2D<> checkPlayerMovement(Entity* player);

	static void setCameraSize(int cameraW, int cameraH);
	static void createProjectile(Vector2D<> pos, Vector2D<> vel, int range, float speed, std::string id,
		Entity* source);
	
private:
	int windowWidth = 0;
	int windowHeight = 0;
	std::filesystem::path assetPath;
	int cnt = 0;
	SDL_Window* window = nullptr;
	uint32_t lastTicks = 0;
	std::unique_ptr<MenuSystem> menu;
	std::optional<Map> map = std::nullopt;
};

#endif
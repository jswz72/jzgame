#ifndef GAME
#define GAME
#include <filesystem>
#include <queue>
#include <vector>
#include "SDL.h"
#include "SDL_Image.h"
#include "AssetManager.h"
#include "ECS/ECS.h"
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
	Game(int windowWidth, int windowHeight);
	~Game() {};

	void loadAssets();
	void initPlayer();
	void initEnemies();
	void initEntities();
	void initUI();
	void init(const char* title, bool fullscreen);
	void handleCollisions();
	void updateCamera();
	void update();
	void render();
	void clean();
	void handleEvents();
	bool running() { return Globals::get().isRunning; }
	void setFpsString(int fps);
	bool playerWillHitWall(const Rect &newPlayerRect, QuadTree &quadTree);
	Vector2D checkPlayerMovement(Entity* player);

	static void setCameraSize(int cameraW, int cameraH);
	static void createProjectile(Vector2D pos, Vector2D vel, int range, float speed, std::string id,
		Entity* source);
	
private:
	int windowWidth = 0;
	int windowHeight = 0;
	std::filesystem::path assetPath;
	int cnt = 0;
	SDL_Window* window = nullptr;
	uint32_t lastTicks = 0;
	MenuSystem* menu = nullptr;
	Map* map;
};

#endif
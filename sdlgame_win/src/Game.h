#ifndef GAME
#define GAME
#include <filesystem>
#include <queue>
#include <vector>
#include "SDL.h"
#include "SDL_Image.h"
#include "AssetManager.h"
#include "KeyboardHandler.h"
#include "MouseHandler.h"
#include "QuadTree.h"
#include "Vector2D.h"

class ColliderComponent;
class AssetManager;

class Game {
public:
	Game(int windowWidth, int windowHeight);
	~Game() {};

	void loadAssets();
	void loadEntities();
	void loadUI();
	void init(const char* title, bool fullscreen);
	void handleCollisions(Vector2D prevPlayerPos);
	void updateCamera();
	void update();
	void render();
	void clean();
	void handleEvents();
	bool running() { return isRunning; }

	static bool isRunning;
	static SDL_Renderer* renderer;
	static SDL_Rect camera;
	static AssetManager* assets;
	static std::vector<ColliderComponent*> colliders;
	static float timeDelta;
	static KeyboardHandler keyboardHandler;
	static MouseButtonHandler mouseButtonHandler;

	enum groupLabels : std::size_t {
		groupMap,
		groupPlayers,
		groupColliders,
		groupProjectiles,
		groupUI,
	};

private:
	int windowWidth = 0;
	int windowHeight = 0;
	std::filesystem::path assetPath;
	int cnt = 0;
	SDL_Window* window = nullptr;
	int lastTicks = 0;
	QuadTree *quadTree = nullptr;
};

#endif
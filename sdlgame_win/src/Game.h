#ifndef GAME
#define GAME
#include <filesystem>
#include <vector>
#include "SDL.h"
#include "SDL_Image.h"
#include "AssetManager.h"
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
	static SDL_Event event;
	static SDL_Rect camera;
	static AssetManager* assets;
	static std::vector<ColliderComponent*> colliders;

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
	SDL_Window* window;
};

#endif
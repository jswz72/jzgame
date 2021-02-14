#ifndef GAME
#define GAME
#include "SDL.h"
#include "SDL_Image.h"
#include "AssetManager.h"
#include <vector>

class ColliderComponent;
class AssetManager;

class Game {
public:
	Game();
	~Game();

	void init(const char* title, int width,
		int height, bool fullscreen);
	void update();
	void render();
	void clean();
	void handleEvents();

	bool running() { return isRunning; }

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static bool isRunning;
	static SDL_Rect camera;
	static AssetManager* assets;

	enum groupLabels : std::size_t {
		groupMap,
		groupPlayers,
		groupColliders,
		groupProjectiles,
	};

	static std::vector<ColliderComponent*> colliders;

private:
	int cnt = 0;
	SDL_Window* window;
};

#endif
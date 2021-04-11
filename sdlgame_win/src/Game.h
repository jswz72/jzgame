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

class ColliderComponent;
class AssetManager;

class Game {
public:
	Game(int windowWidth, int windowHeight);
	~Game() {};

	void loadAssets();
	void initPlayer();
	void initEntities();
	void initUI();
	void init(const char* title, bool fullscreen);
	void handleCollisions(Vector2D prevPlayerPos);
	void updateCamera();
	void update();
	void render();
	void clean();
	void handleEvents();
	bool running() { return isRunning; }
	void setFpsString(int fps);

	static void setCameraSize(int cameraW, int cameraH);
	static void createProjectile(Vector2D pos, Vector2D vel, int range, float speed, std::string id,
		Entity* source);

	static bool isRunning;
	static bool isPaused;
	static bool debug;
	static SDL_Renderer* renderer;
	static SDL_Rect camera;
	static AssetManager assetManager;
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
	QuadTree* quadTree = nullptr;
	MenuSystem* menu = nullptr;
};

#endif
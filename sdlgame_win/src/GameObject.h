#include <cstdint>
#include "SDL.h"
#include "SDL_Image.h"
#include "Game.h"

class GameObject {
public:
	GameObject(const char* textureSheet, int x, int y, SDL_Rect* rect = nullptr,
		uint32_t RGBVals[] = nullptr);
	~GameObject();
	void update();
	void render();
	void setSrcRect(SDL_Rect rect);

private:
	int xPos = 0;
	int yPos = 0;

	SDL_Texture* objTexture = nullptr;
	SDL_Rect srcRect{};
	SDL_Rect dstRect{};
	SDL_Renderer* renderer = nullptr;
};
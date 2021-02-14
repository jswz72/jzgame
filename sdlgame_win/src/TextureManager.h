#ifndef TEXTUREMAN
#define TEXTUREMAN
#include <filesystem>
#include <cstdint>
#include <SDL.h>
#include "SDL_image.h"
#include "Game.h"


class TextureManager {
public:
	static SDL_Texture* loadTexture(std::filesystem::path path, uint32_t RGBVals[] = nullptr);
	static void draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dst, SDL_RendererFlip flip);
};
#endif
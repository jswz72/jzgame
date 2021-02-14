#include "TextureManager.h"
#include <stdexcept>
#include <iostream>

SDL_Texture* TextureManager::loadTexture(std::filesystem::path path, uint32_t RGBVals[]) {
	SDL_Surface* tmpSurface = IMG_Load(path.string().c_str());
	if (!tmpSurface) {
		std::cout << IMG_GetError() << std::endl;
		throw std::exception("Error loading image");
	}
	if (RGBVals) {
		SDL_SetColorKey(tmpSurface, SDL_TRUE, SDL_MapRGB(tmpSurface->format,
			RGBVals[0], RGBVals[1], RGBVals[2]));
	}
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	return tex;
}

void TextureManager::draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dst, SDL_RendererFlip flip) {
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dst, 0, NULL, flip);
}
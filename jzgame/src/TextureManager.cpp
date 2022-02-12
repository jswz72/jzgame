#include "TextureManager.h"
#include <stdexcept>
#include <iostream>
#include "Globals.h"

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
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Globals::get().renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	return tex;
}

void TextureManager::draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dst, SDL_RendererFlip flip) {
	auto retVal = SDL_RenderCopyEx(Globals::get().renderer, tex, &src, &dst, 0, NULL, flip);
	if (retVal == -1) {
		throw std::exception("Error doing render copy");
	}
}
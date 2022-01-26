#include "UILabel.h"

#include <cassert>

void UILabel::setLabelText(std::string text, std::string fontName) {
	labelText = text;
	labelFont = fontName;

	auto font = Globals::get().assetManager.getFont(fontName);
	SDL_DestroyTexture(labelTexture);
	if (text.empty()) {
		return;
	}
	SDL_Surface* surf = TTF_RenderText_Blended(font, labelText.c_str(), textColor);
	labelTexture = SDL_CreateTextureFromSurface(Globals::get().renderer, surf);
	assert(labelTexture);
	SDL_FreeSurface(surf);

	SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
}
#pragma once
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "../AssetManager.h"
#include "ECS.h"
#include "../Game.h"
#include "../TextureManager.h"

class UILabel : public Component {
public:
	UILabel(int xpos, int ypos, std::string text, std::string font,
		SDL_Color& color) : labelText(text), labelFont(font), textColor(color) {
		position.x = xpos;
		position.y = ypos;

		setLabelText(text, font);
	}
	~UILabel() {}

	void setLabelText(std::string text, std::string fontName) {
		labelText = text;
		labelFont = fontName;

		auto font = Game::assets->getFont(fontName);
		SDL_Surface* surf = TTF_RenderText_Blended(font, labelText.c_str(), textColor);
		labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
		SDL_FreeSurface(surf);

		SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
	}

	void draw() override {
		SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
	}
private:
	SDL_Rect position;
	std::string labelText;
	std::string labelFont;
	SDL_Color textColor;
	SDL_Texture* labelTexture;
};
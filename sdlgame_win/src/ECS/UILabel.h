#pragma once
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "../AssetManager.h"
#include "../Game.h"
#include "../TextureManager.h"
#include "ECS.h"

class UILabel : public Component {
public:
	UILabel(int xpos, int ypos, std::string text, std::string font,
		SDL_Color& color) : labelText(text), labelFont(font), textColor(color) {
		position.x = xpos;
		position.y = ypos;
		setLabelText(text, font);
	}

	void setLabelText(std::string text, std::string fontName);

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
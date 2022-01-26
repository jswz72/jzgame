#pragma once
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "../AssetManager.h"
#include "../Globals.h"
#include "../TextureManager.h"
#include "ECS.h"

class UILabel : public Component {
public:
	UILabel(int xpos, int ypos, std::string text, std::string font,
		SDL_Color& color, bool debug=false)
			: labelText(text), labelFont(font), textColor(color), debug(debug) {
		position.x = xpos;
		position.y = ypos;
		setLabelText(text, font);
	}

	void setLabelText(std::string text, std::string fontName);

	void draw() override {
		if (debug && !Globals::get().debug) {
			return;
		}
		SDL_RenderCopy(Globals::get().renderer, labelTexture, nullptr, &position);
	}
private:
	// Label is debug-only, shouldn't show during normal gameplay.
	bool debug;
	SDL_Rect position;
	std::string labelText;
	std::string labelFont;
	SDL_Color textColor;
	SDL_Texture* labelTexture;
};
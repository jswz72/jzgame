#pragma once
#include <cstdint>

struct RGBVals {
	RGBVals(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0) : r(red),
		g(green), b(blue) {};
	uint8_t r;
	uint8_t g;
	uint8_t b;

	static RGBVals red() {
		return RGBVals(255);
	}
	static RGBVals green() {
		return RGBVals(0, 255, 0);
	}
	static RGBVals blue() {
		return RGBVals(0, 0, 255);
	}
	static RGBVals white() {
		return RGBVals(255, 255, 255);
	}
	static RGBVals gray() {
		return RGBVals(128, 128, 128);
	}
	static RGBVals turquoise() {
		return RGBVals(0, 255, 255);
	}
};

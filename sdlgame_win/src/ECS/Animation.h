#pragma once

struct Animation {
	int index = 0;
	int frames = 0;
	int speed = 0;

	Animation() {}
	Animation(int i, int f, int s) : index(i), frames(f), speed(s) {}
};
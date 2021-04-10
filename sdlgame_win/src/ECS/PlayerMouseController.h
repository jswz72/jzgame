#pragma once

#include "ECS.h"

class PlayerMouseController : public Component {
public:
	void update() override;

private:
	unsigned int lastFire = 0;
	// Interval between shots in ms.
	const unsigned int fireInterval = 200;
};

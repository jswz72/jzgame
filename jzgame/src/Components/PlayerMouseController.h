#pragma once

#include "../EntityComponent.h"

class PlayerMouseController : public Component {
public:
	void update() override;

private:
	unsigned int lastFire = 0;
	// Interval between shots in ms.
	unsigned int fireInterval = 200;
};

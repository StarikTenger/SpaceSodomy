#pragma once
#include "Creature.h"
#include "Shooter.h"

class RocketLauncher : public Creature, public Shooter{
public:
	RocketLauncher();
	~RocketLauncher();
};


#pragma once
#include "Unit.h"
class Explosion : public Unit {
public:
	double damage = 1;
	double explosionVel = 8;
	double maxR = 1;
	Explosion();
	~Explosion();
};


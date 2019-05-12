#include "Explosion.h"
#include <iostream>

Explosion::Explosion()
{
	collision = 0;
	hp = 1000000000;
	team = "evil";
	//std::cout << "e";
}

Explosion::~Explosion()
{
}

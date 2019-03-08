#include "RocketLauncher.h"

RocketLauncher::RocketLauncher() {
	body.w = 2;
	orders.shoot = 1;
	gun.cooldownTime = 0.2;
	gun.bulletVelocity = 5;
	team = "enemy";
	shields = 5;
	body.m = 100;
}

RocketLauncher::~RocketLauncher()
{
}

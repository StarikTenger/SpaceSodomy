#include "System.h"
#include "geometry.h"
#include "random.h"

#include <iostream>

using namespace geom;
using namespace random;

bool System::checkAbility(Unit* shooter, Unit* target, double threshold) {
	if (distance(shooter->body.pos, target->body.pos)<EPS) {
		return 0;
	}
	bool contact = 1;

	double stepSize = 0.5;
	double a = abs(angle(shooter->body.vel - target->body.vel + direction(shooter->body.direction) * dynamic_cast<Shooter*>(shooter)->gun.bulletVelocity)
		- angle(target->body.pos - shooter->body.pos));
	Vector2d step = direction(angle(shooter->body.vel - target->body.vel + direction(shooter->body.direction) * dynamic_cast<Shooter*>(shooter)->gun.bulletVelocity)) * stepSize;
	for (int i = 0; i < distance(shooter->body.pos, target->body.pos) / stepSize; i++) {
		if (checkWall(shooter->body.pos + step * i)) {
			contact = 0;
		}
	}
	
	if (!contact) {
		return 0;
	}


	
	while (a >= 2 * M_PI) {
		a -= 2 * M_PI;
	}
	while (a <= 0) {
		a += 2 * M_PI;
	}
	if (a < threshold) {
		return 1;
	}
	return 0;
}
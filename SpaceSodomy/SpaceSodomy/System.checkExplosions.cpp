#include "System.h"
#include "geometry.h"
#include "random.h"

#include <vector>
#include <algorithm>
#include <iostream>

#include "getMilliCount.h"

using namespace geom;
using namespace random;

void System::checkExplosions(Unit* c) {
	double k = 1;
	double d = 0.05;
	if (dynamic_cast<LaserCarrier*>(c) || dynamic_cast<RocketLauncher*>(c))
		return;
	for (Unit* u : units) {
		
		double r = distance(c->body.pos, u->body.pos);
		if (!(r < c->body.r + u->body.r))
			continue;
		if (c == u)
			continue;
		if (!dynamic_cast<Explosion*>(u))
			continue;
		

		c->body.vel += direction(c->body.pos, u->body.pos) * k / c->body.m / r / r * dt;
		
		
	}
}
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
	for (Unit* u : units) {
		Explosion* e;
		if (!(e = dynamic_cast<Explosion*>(u)) || distance(c->body.pos, u->body.pos) < EPS)
			continue;
		if (dynamic_cast<LaserCarrier*>(u) || dynamic_cast<RocketLauncher*>(u))
			continue;
		double r = distance(c->body.pos, u->body.pos);
		if (r < c->body.r + u->body.r) {
			c->body.vel += direction(c->body.pos, u->body.pos) * k / c->body.m / r / r * dt;
		}
	}
}
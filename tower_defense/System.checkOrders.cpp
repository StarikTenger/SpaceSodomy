#include "System.h"
#include "geometry.h"
#include "random.h"
#include "getMilliCount.h"

#include <iostream>

using namespace geom;
using namespace random;
using namespace std;

void System::checkOrders(Creature* c) {
	Mover* s;
	if (s = dynamic_cast<Ship*>(c)) {
		if (c->orders.forward) {
			c->body.vel += direction(c->body.direction)*s->engine.mainForce / c->body.m * dt;
		}
		if (c->orders.backward) {
			c->body.vel -= direction(c->body.direction)*s->engine.mainForce / c->body.m * dt;
		}
		if (c->orders.right) {
			c->body.vel += direction(c->body.direction + M_PI/2)*s->engine.mainForce / c->body.m * dt;
		}
		if (c->orders.left) {
			c->body.vel += direction(c->body.direction - M_PI / 2)*s->engine.mainForce / c->body.m * dt;
		}
		if (c->orders.turnRight) {
			c->body.w += s->engine.turnForce / c->body.m * dt;
		}
		if (c->orders.turnLeft) {
			c->body.w -= s->engine.turnForce / c->body.m * dt;
		}
	}
	Shooter* t;
	if (t = dynamic_cast<Shooter*>(c)) {
		if (c->orders.shoot) {
			if (t->gun.timeToCooldown <= 0) {
				auto angles = angleDistribution(c->body.direction, t->gun.divergenceAngle, t->gun.directions);
				for (double a : angles) {
					Bullet* b = new Bullet;
					b->body = c->body;
					b->body.wetFrictionK = 0;
					b->body.r = 0.1;
					b->body.vel += direction(a)*t->gun.bulletVelocity;
					b->body.direction = a;
					b->body.w = 0;
					b->team = c->team;
					additionalUnits.push_back(b);
					t->gun.timeToCooldown = t->gun.cooldownTime;
					sound("launch", c->body.pos, 1);
				}
			}
		}
	}
}
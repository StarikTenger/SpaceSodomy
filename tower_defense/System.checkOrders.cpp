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
			//sound("fire", c->body.pos, 1);
			c->body.vel += direction(c->body.direction)*s->engine.mainForce / c->body.m * dt;
		}
		if (c->orders.backward) {
			//sound("fire", c->body.pos, 1);
			c->body.vel -= direction(c->body.direction)*s->engine.mainForce / c->body.m * dt;
		}
		if (c->orders.right) {
			//sound("fire", c->body.pos, 1);
			c->body.w += s->engine.turnForce / c->body.m * dt;
		}
		if (c->orders.left) {
			//sound("fire", c->body.pos, 1);
			c->body.w -= s->engine.turnForce / c->body.m * dt;
		}
	}
	Shooter* t;
	if (t = dynamic_cast<Shooter*>(c)) {
		if (c->orders.shoot) {
			if (t->gun.timeToCooldown <= 0) {
				Bullet* b = new Bullet;
				b->body = c->body;
				b->body.r = 0.1;
				b->body.vel += direction(c->body.direction)*t->gun.bulletVelocity;
				b->team = c->team;
				additionalUnits.push_back(b);
				t->gun.timeToCooldown = t->gun.cooldownTime;
				sound("launch", c->body.pos, 1);
			}
		}
	}
}
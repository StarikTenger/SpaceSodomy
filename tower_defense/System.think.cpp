#include "System.h"
#include "geometry.h"
#include "random.h"

#include <iostream>

using namespace geom;
using namespace random;

void System::think(Creature* c){
	Turret* t;
	Ship* target;
	if (!(target = dynamic_cast<Ship*>(units[0])))
		return;
	if (t = dynamic_cast<Turret*>(c)) {
		bool contact = 1;
		if (distance(t->body.pos, target->body.pos) > t->activeRadius) {
			contact = 0;
		}
		else {
			double stepSize = 0.5;
			Vector2d step = direction(target->body.pos, t->body.pos) * stepSize;
			for (int i = 0; i < distance(t->body.pos, target->body.pos) / stepSize; i++) {
				if (checkWall(t->body.pos + step * i)) {
					contact = 0;
				}
			}
		}
		if (!contact) {
			t->anger -= dt;
			if (t->anger < 0) {
				t->anger = 0;
			}
			return;
		}
		t->anger += dt;
		if (t->anger < t->angerThreshold) {
			return;
		}
		if (t->anger - dt < t->angerThreshold) {
			sound("bip", c->body.pos, 100);
			//std::cout << "a";
		}
	
	
		double a = abs(angle(t->body.vel - target->body.vel + direction(t->body.direction) * t->gun.bulletVelocity)
			- angle(target->body.pos - t->body.pos));
		while (a >= 2*M_PI) {
			a -= 2*M_PI;
		}
		while (a <= 0) {
			a += 2 * M_PI;
		}
		t->orders.shoot = 0;
		if ( a < 0.02 ) {
			t->orders.shoot = 1;
		}
	}
}
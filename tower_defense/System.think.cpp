#include "System.h"
#include "geometry.h"
#include "random.h"

#include <iostream>

using namespace geom;
using namespace random;

void System::think(Creature* c){
	Turret* t;
	Robot* r;
	if (t = dynamic_cast<Turret*>(c)) {
		think(t);
	}
	if (r = dynamic_cast<Robot*>(c)) {
		think(r);
	}
}

void System::think(Turret* t) {
	Ship* target;
	if (!(target = dynamic_cast<Ship*>(units[0])) && dynamic_cast<Robot*>(units[0]))
		return;
	
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
		sound("bip", t->body.pos, 100);
		//std::cout << "a";
	}


	double a = abs(angle(t->body.vel - target->body.vel + direction(t->body.direction) * t->gun.bulletVelocity)
		- angle(target->body.pos - t->body.pos));
	while (a >= 2 * M_PI) {
		a -= 2 * M_PI;
	}
	while (a <= 0) {
		a += 2 * M_PI;
	}
	t->orders.shoot = 0;
	if (a < 0.02) {
		t->orders.shoot = 1;
	}
}
void System::think(Robot* r) {
	Ship* target;
	if (!(target = dynamic_cast<Ship*>(units[0])) && dynamic_cast<Robot*>(units[0]))
		return;
	bool contact = 1;
	double stepSize = 0.5;
	Vector2d step = direction(target->body.pos, r->body.pos) * stepSize;
	for (int i = 0; i < distance(r->body.pos, target->body.pos) / stepSize; i++) {
		if (checkWall(r->body.pos + step * i)) {
			contact = 0;
		}
	}

	double a = abs(angle(r->body.vel - target->body.vel + direction(r->body.direction) * r->gun.bulletVelocity)
		- angle(target->body.pos - r->body.pos));
	while (a >= 2 * M_PI) {
		a -= 2 * M_PI;
	}
	while (a <= 0) {
		a += 2 * M_PI;
	}
	r->orders.shoot = 0;
	if (a < 0.02) {
		r->orders.shoot = 1;
	}

	r->orders.forward = 0;
	for (auto u : units) {
		Bullet* b = dynamic_cast<Bullet*>(u);
		if (dynamic_cast<Bullet*>(u)) {
			
			double a = abs(angle(r->body.vel - b->body.vel)
				- angle(b->body.pos - r->body.pos));
			while (a >= 2 * M_PI) {
				a -= 2 * M_PI;
			}
			while (a <= 0) {
				a += 2 * M_PI;
			}
			r->orders.forward = 0;
			if (a < 0.2) {
				r->orders.forward = 1;
				std::cout << "Bullet\n";
			}
		}
	}
}

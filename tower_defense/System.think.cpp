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
	t->orders.shoot = 0;
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
	if (a < 0.02) {
		t->orders.shoot = 1;
	}
}
void System::think(Robot* r) {
	r->fear -= dt;
	if (r->fear < -1 && r->fear + dt >= -1)
		r->characteristic = random::intRandom(0, 16000);
	for (Unit* target : units) {
		if (target->team == r->team || (!dynamic_cast<Ship*>(target) && !dynamic_cast<Bullet*>(target)))
			continue;
		bool contact = 1;
		double stepSize = 0.5;
		Vector2d step = direction(r->body.direction) * stepSize;
		for (int i = 0; i < distance(r->body.pos, target->body.pos) / stepSize; i++) {
			if (checkWall(r->body.pos + step * i)) {
				contact = 0;
				break;
				//std::cout << "no contact\n";
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
		if (a < 0.02 && contact) {
			r->orders.shoot = 1;
		}
	}

	r->orders.forward = 0;
	r->engine.directMode = 1;
	for (auto u : units) {
		Bullet* b = dynamic_cast<Bullet*>(u);
		if (dynamic_cast<Bullet*>(u)) {
			if (u->team == r->team)
				continue;
			double a = abs(angle(r->body.vel - b->body.vel)
				- angle(b->body.pos - r->body.pos));
			while (a >= 2 * M_PI) {
				a -= 2 * M_PI;
			}
			while (a <= 0) {
				a += 2 * M_PI;
			}
			
			if (a < (r->body.r*2 + b->body.r) / distance(r->body.pos, b->body.pos)) {
				r->orders.forward = 1;
				r->fear = 0.2;
				double da = M_PI * 0.5;
				if ((r->characteristic/2) % 2) {
					da = -da;
				}
				r->engine.direction = angle(b->body.pos - r->body.pos) - r->body.direction + da;
			}
		}
	}
	if (r->fear <= 0) {
		double radius = 1;
		if (checkWall(r->body.pos + Vector2d(radius, 0))) {
			r->orders.forward = 1;
			r->engine.direction = -r->body.direction + M_PI;
		} 
		else if (checkWall(r->body.pos + Vector2d(-radius, 0))) {
			r->orders.forward = 1;
			r->engine.direction = -r->body.direction;
		}
		else if (checkWall(r->body.pos + Vector2d(0, radius))) {
			r->orders.forward = 1;
			r->engine.direction = -r->body.direction + M_PI*1.5;
		}
		else if (checkWall(r->body.pos + Vector2d(0, -radius))) {
			r->orders.forward = 1;
			r->engine.direction = -r->body.direction + M_PI * 0.5;
		}
		else if (checkWall(r->body.pos + Vector2d(radius, radius))) {
			r->orders.forward = 1;
			r->engine.direction = -r->body.direction + M_PI * 1.25;
		}
		else if (checkWall(r->body.pos + Vector2d(-radius, radius))) {
			r->orders.forward = 1;
			r->engine.direction = -r->body.direction + M_PI * 1.75;
		}
		else if (checkWall(r->body.pos + Vector2d(-radius, -radius))) {
			r->orders.forward = 1;
			r->engine.direction = -r->body.direction + M_PI * 0.25;
		}
		else if (checkWall(r->body.pos + Vector2d(radius, -radius))) {
			r->orders.forward = 1;
			r->engine.direction = -r->body.direction + M_PI * 0.75;
		}
		else if (distance(units[0]->body.pos, r->body.pos) < 4) {
			if(random::intRandom(0, 4) == 0)
				r->orders.forward = 1;
			double deviation = double(r->characteristic % 100) / 400 * M_PI;
			double da = M_PI * 0.5 + deviation;
			
			if (r->characteristic % 2) {
				da = -da;
			}
			r->engine.direction = angle(units[0]->body.pos - r->body.pos) - r->body.direction + da;
		}else 
		if (distance({}, r->body.vel) > EPS * 100) {
			r->orders.forward = 1;
			r->engine.direction = -r->body.direction + angle(r->body.vel) + M_PI;
		}
	}
}

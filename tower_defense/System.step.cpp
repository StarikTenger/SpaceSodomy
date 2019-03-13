#include "System.h"
#include "geometry.h"
#include "random.h"

#include <vector>
#include <algorithm>
#include <iostream>

#include "getMilliCount.h"

using namespace geom;
using namespace random;
using namespace std;



void System::step() {
	time += dt;
	//collision
	collision();
	fillChunks();

	//movement
	for (Unit* u : units) {
		auto& b = u->body;
		b.pos += b.vel*dt;
		b.direction += b.w*dt;
		b.posPrev = b.pos;
		b.velPrev = b.vel;

	}

	


	//bullet-check
	for (Unit* u : units) {
		if (dynamic_cast<Bullet*>(u) || dynamic_cast<Explosion*>(u)) {
			auto neighbors = getNeighbors(u);
			for (Unit* u1 : neighbors) {
				if (dynamic_cast<Creature*>(u1) || dynamic_cast<Bullet*>(u1) || dynamic_cast<Dummy*>(u1)) {
					if (distance(u->body.pos, u1->body.pos) < u->body.r + u1->body.r && u->team != u1->team) {
						bool dmg = 0;
						if (!dynamic_cast<Explosion*>(u)) {
							u->hp -= 100;
							dmg = 1;
						}
						else if (distance(u->body.pos, u1->body.pos) < u->body.r / 2) {
							dmg = 1;
						}
						if (dmg && dynamic_cast<Creature*>(u1) && dynamic_cast<Creature*>(u1)->immortality <= 0) {
							if (dynamic_cast<Ship*>(u1)) {
								events.push_back("damage");
							}
							if (dynamic_cast<Creature*>(u1)->shields) {
								dynamic_cast<Creature*>(u1)->shields--;
								dynamic_cast<Creature*>(u1)->immortality = 0.2;
							}
							else {
								u1->hp -= 100;
							}
						}
					}
				}
			}
		}
		if (dynamic_cast<Exit*>(u)) {
			if (distance(u->body.pos, units[0]->body.pos) < u->body.r + units[0]->body.r) {
				status = "next level";
			}
		}
	}
	for (Unit* u : units) {
		auto neighbors = getNeighbors(u);
		for (Unit* u1 : neighbors) {
			Bonus* bonus = dynamic_cast<Bonus*>(u1);
			Creature* creature = dynamic_cast<Creature*>(u);
			if (bonus && creature && distance(u->body.pos, u1->body.pos) <= u->body.r + u1->body.r && bonus->hp > 0) {
				if (bonus->type == "shield") {
					bonus->hp = 0;
					creature->shields++;
				}
			}
		}
	}


	//orders	
	for (Unit* u : units) {
		Creature* c;
		if (c = dynamic_cast<Creature*>(u)){
			checkOrders(c);
		}
		
	}
	for (Unit* u : additionalUnits) {
		units.push_back(u);
	}
	additionalUnits = {};

	//personal managment
	for (Unit* u : units) {
		Creature* c;
		Shooter* s;
		if (s = dynamic_cast<Shooter*>(u)) {
			s->gun.timeToCooldown -= dt;
		}
		if (c = dynamic_cast<Creature*>(u)) {
			think(c);
			c->immortality -= dt;
		}
		Bullet* b;
		if (b = dynamic_cast<Bullet*>(u)) {
			b->hp -= dt;
			if (b->hp < 0) {
				Explosion* e = new Explosion();
				e->body = u->body;
				e->body.w = 0;
				e->body.vel = e->body.vel / 2;
				additionalUnits.push_back(e);
				sound("explosion", u->body.pos, 1);
			}
		}
		Explosion* e;
		if (e = dynamic_cast<Explosion*>(u)) {
			e->body.r += dt * e->explosionVel;
			if (e->body.r > e->maxR) {
				e->hp = -EPS;
			}
		}
	}

	//killing
	for (int i = 0; i < units.size(); i++) {
		if (units[i]->hp <= 0) {
			if (i == 0) {
				start();
			}
			else if (!dynamic_cast<Creature*>(units[i])) {
				delete units[i];
				units.erase(units.begin() + i);
				i--;
			}
			else {
				sound("death", units[i]->body.pos, 100);
				Dummy* d = new Dummy();
				d->body = units[i]->body;
				if (dynamic_cast<Turret*>(units[i])) {
					d->type = "turret";
				}
				if (dynamic_cast<RocketLauncher*>(units[i])) {
					d->type = "rocketLauncher";
				}
				delete units[i];
				units[i] = d;
			}
		}
	}
}
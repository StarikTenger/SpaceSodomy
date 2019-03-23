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
	fillChunks();
	collision();
	

	//movement
	for (Unit* u : units) {
		auto& b = u->body;
		//b.vel -= b.vel*wetFrictionK*b.wetFrictionK*dt;
		if (dynamic_cast<Ship*>(u)) {
			//b.w -= b.w*wetFrictionK*b.wetFrictionK*dt;
		}
		b.pos += b.vel*dt;
		b.direction += b.w*dt;
		b.posPrev = b.pos;
		b.velPrev = b.vel;
	}




	//bullet-check
	for (Unit* agressor : units) {
		if (dynamic_cast<Bullet*>(agressor) || dynamic_cast<Explosion*>(agressor) || dynamic_cast<LaserCarrier*>(agressor)) {
			auto neighbors = getNeighbors(agressor);
			for (Unit* victim : units) {
				if (dynamic_cast<LaserCarrier*>(agressor)) {
					LaserCarrier* laserCarrier = dynamic_cast<LaserCarrier*>(agressor);
					for (auto laser : laserCarrier->lasers) {
						double a = laserCarrier->body.direction + laser.direction +M_PI/2;
						Vector2d r = direction(a) *  victim->body.r;
						if (agressor->team != victim->team && isCross(laser.end, laser.base, victim->body.pos - r, victim->body.pos + r)) {
							damage(victim);
							
						}
						
					}
				} else 
				if (dynamic_cast<Creature*>(victim) || dynamic_cast<Bullet*>(victim) || dynamic_cast<Dummy*>(victim)) {
					if (distance(agressor->body.pos, victim->body.pos) < agressor->body.r + victim->body.r && agressor->team != victim->team) {
						bool dmg = 0;
						if (!dynamic_cast<Explosion*>(agressor)) {
							damage(agressor);
							dmg = 1;
						}
						else if (distance(agressor->body.pos, victim->body.pos) < agressor->body.r / 2) {
							dmg = 1;
						}
						if (dmg && dynamic_cast<Creature*>(victim) && victim->hp > 0) {
							damage(victim);
						}
					}
				}
			}
		}
		if (dynamic_cast<Exit*>(agressor)) {
			if (distance(agressor->body.pos, units[0]->body.pos) < agressor->body.r + units[0]->body.r) {
				status = "next level";
			}
		}
	}

	//bonuses
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
		if (c = dynamic_cast<Creature*>(u)) {
			checkOrders(c);
		}

	}


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
		LaserCarrier* lC;
		if (lC = dynamic_cast<LaserCarrier*>(u)) {
			for (auto& laser : lC->lasers) {
				Vector2d pos = lC->body.pos;
				laser.base = pos;
				double a = lC->body.direction + laser.direction;
				Vector2d dir = direction(a);
				
				for (int i = 0; i < 100; i++) {
					if (checkWall(pos)) {
						double x = (int)(pos.x / blockSize + field.size()) - field.size();
						double y = (int)(pos.y / blockSize + field[0].size()) - field[0].size();
						double width = field.size();
						double height = field[0].size();
						Vector2d centre(x + 0.5, y + 0.5);
						double r = 0.55;
						if (isCross(centre + Vector2d(r, r), centre + Vector2d(r, -r), laser.base, pos)) {
							laser.end = getCross(centre + Vector2d(r, r), centre + Vector2d(r, -r), laser.base, pos);
						} else 
						if (isCross(centre + Vector2d(-r, r), centre + Vector2d(-r, -r), laser.base, pos)) {
							laser.end = getCross(centre + Vector2d(-r, r), centre + Vector2d(-r, -r), laser.base, pos);
						} else
						if (isCross(centre + Vector2d(r, r), centre + Vector2d(-r, r), laser.base, pos)) {
							laser.end = getCross(centre + Vector2d(r, r), centre + Vector2d(-r, r), laser.base, pos);
						} else
						if (isCross(centre + Vector2d(-r, -r), centre + Vector2d(r, -r), laser.base, pos)) {
							laser.end = getCross(centre + Vector2d(-r, -r), centre + Vector2d(r, -r), laser.base, pos);
						} else if (isCross(Vector2d(0, 0), Vector2d(width, 0), laser.base, pos)) {
							laser.end = getCross(Vector2d(0, 0), Vector2d(width, 0), laser.base, pos);
						} else 
						if (isCross(Vector2d(0, 0), Vector2d(0, height), laser.base, pos)) {
							laser.end = getCross(Vector2d(0, 0), Vector2d(0, height), laser.base, pos);
						} else
						if (isCross(Vector2d(width, height), Vector2d(width, 0), laser.base, pos)) {
							laser.end = getCross(Vector2d(width, height), Vector2d(width, 0), laser.base, pos);
						} else
						if (isCross(Vector2d(width, height), Vector2d(0, height), laser.base, pos)) {
							laser.end = getCross(Vector2d(width, height), Vector2d(0, height), laser.base, pos);
						} else
						if (isCross(Vector2d(0, 0), Vector2d(width, 0), laser.base, pos)) {
							laser.end = getCross(Vector2d(0, 0), Vector2d(width, 0), laser.base, pos);
						} else {
							laser.end = pos;
						}
						break;
					}
					pos += dir * 0.5;
				}
			}
		}
	}
	for (Unit* u : additionalUnits) {
		units.push_back(u);
	}
	additionalUnits = {};

	

	//killing
	for (int i = 0; i < units.size(); i++) {
		if (units[i]->hp <= 0) {
			if (i == 0) {
				//start();
				status = "death";
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
				d->hp = 100000;
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
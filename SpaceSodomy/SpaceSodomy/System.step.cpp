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

	// setting colors
	for (int& c : colorsActive) {
		c = 0;
	}
	colorsActive[0] = 1;
	

	// collision
	fillChunks();
	collision();
	

	// movement
	for (Unit* u : units) {
		auto& b = u->body;
		
		b.pos += b.vel*dt;
		b.direction += b.w*dt;
		b.posPrev = b.pos;
		b.velPrev = b.vel;
	}

	// bullet-check
	// lasers
	for (Unit* agressor : units) {
		if (dynamic_cast<Bullet*>(agressor) || dynamic_cast<Explosion*>(agressor) || dynamic_cast<LaserCarrier*>(agressor)) {
			auto neighbors = getNeighbors(agressor);
			for (Unit* victim : units) {
				if (dynamic_cast<LaserCarrier*>(agressor)) {
					LaserCarrier* laserCarrier = dynamic_cast<LaserCarrier*>(agressor);
					for (auto laser : laserCarrier->lasers) {
						double a = laserCarrier->body.direction + laser.direction + M_PI / 2;
						Vector2d r = direction(a) *  victim->body.r;
						if (isCross(laser.end, laser.base, victim->body.pos - r, victim->body.pos + r) && !dynamic_cast<LaserCarrier*>(victim)) {
							if(!dynamic_cast<Bonus*>(victim))
								damage(victim);
						}

					}
				}
			}
		}
	}
	// bullets
	for (Unit* agressor : units) {
		if (dynamic_cast<Bullet*>(agressor) || dynamic_cast<Explosion*>(agressor) || dynamic_cast<LaserCarrier*>(agressor)) {
			auto neighbors = getNeighbors(agressor);
			for (Unit* victim : neighbors) {
				if (dynamic_cast<Creature*>(victim) || dynamic_cast<Bullet*>(victim) || dynamic_cast<Dummy*>(victim)) {
					if (distance(agressor->body.pos, victim->body.pos) < agressor->body.r + victim->body.r && agressor->team != victim->team) {
						bool dmg = 0;
						if (!dynamic_cast<Bullet*>(agressor)) {
							damage(agressor);
							damage(victim);
							dmg = 1;
						}
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

	// bonuses
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

	// personal managment (think, particles)
	for (Unit* u : units) {
		Shooter* s;
		if (s = dynamic_cast<Shooter*>(u)) {
			s->gun.timeToCooldown -= dt;
		}
		Creature* c;
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
				for (int i = 0; i < 50 * particleLevel; i++) {
					double duration = floatRandom(0.5, 1, 3)/2;
					Vector2d pos = direction(floatRandom(0, 2 * M_PI, 3)) * floatRandom(0, 2, 3) + e->body.vel * duration;
					animation("particleFlame",
						AnimationState(e->body.pos, Vector2d(0.2, 0.2), e->body.direction, Color(255, 255, 255)),
						AnimationState(e->body.pos + pos, Vector2d(0, 0), e->body.direction, Color(255, 255, 255)),
						duration
					);
				}
			}
			double d = 0.1;
			double dist = 0;
			double r = 0.2;
			Vector2d dp = direction(floatRandom(0, 2 * M_PI, 3)) * floatRandom(0, 0.2, 3);
			if (checkTime(particlePeriod)) {
				animation("particleSmoke",
					AnimationState(u->body.pos, Vector2d(r, r), u->body.direction, Color(255, 255, 255)),
					AnimationState(u->body.pos + dp + u->body.vel*d * 0 - direction(u->body.direction + floatRandom(-0.1, 0.1, 3)) *dist, Vector2d(r, r), u->body.direction, Color(255, 255, 255, 0)),
					d);
				
			}
		}
		Explosion* e;
		if (e = dynamic_cast<Explosion*>(u)) {
			e->body.r += dt * e->explosionVel;
			if (e->body.r > e->maxR) {
				e->hp = -EPS;
			}
		}
		LaserCarrier* lC; //laser crossing
		if (lC = dynamic_cast<LaserCarrier*>(u)) {
			for (auto& laser : lC->lasers) {
				Vector2d pos = lC->body.pos;
				laser.base = lC->body.pos;
				double a = lC->body.direction + laser.direction;
				Vector2d dir = direction(a);
				double stepSize = 0.5;
				
				for (int i = 0; i < 100; i++) {
					if (checkWall(pos)) {
						double x = (double)(int)(pos.x / blockSize + field.size()) - field.size();
						double y = (double)(int)(pos.y / blockSize + field[0].size()) - field[0].size();
						double width = field.size();
						double height = field[0].size();
						Vector2d centre(x + 0.5, y + 0.5);
						Vector2d rel = pos - centre; // relative coordinates
						double r = 0.5;
						// borders
						if (isCross(Vector2d(0, 0), Vector2d(width, 0), laser.base, pos)) {
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
						} 
						else {
							laser.end = pos;
							if (stepSize > 0.02) {
								pos -= dir * stepSize;
								stepSize /= 2;
								continue;
							}
						}
						break;
					}
					pos += dir * stepSize;
				}
				for (int i = 0; i < 1; i++) {
					if (checkTime(particlePeriod)) {
						double d = 0.3;
						double dist = 1;
						double r = 0.3;
						double disp = 1;
						double dir = angle(laser.end - laser.base) + floatRandom(-disp, disp, 3);
						Vector2d dp = direction(angle(laser.end - laser.base) + M_PI/2) * floatRandom(-0.2, 0.2, 3);
						if (checkTime(particlePeriod)) {
							animation("particleLaser",
								AnimationState(laser.end + dp, Vector2d(0, r), dir, Color(255, 255, 255)),
								AnimationState(laser.end + dp - direction(dir) *dist,
									Vector2d(r, 0), dir, Color(255, 255, 255)),
								d);
						}
					}
				}
			}
		}
		Generator* g;
		if (g = dynamic_cast<Generator*>(u)) {
			colorsActive[g->color] = 1;
			Vector2d pos = geom::direction(random::floatRandom(0, M_PI*2, 3))*random::floatRandom(0, 0.5, 3);
			Color color = colorMatches[g->color];
			if (checkTime(particlePeriod) && !random::intRandom(0, 6))
				animation("particleFlame",
					AnimationState(g->body.pos + pos, Vector2d(0.0, 0.0), 0, colorMatches[g->color]),
					AnimationState(g->body.pos + pos, Vector2d(0.2, 0.2), 0, Color(color.r, color.g, color.b, 0)),
					1
				);
		}
	}

	// orders	
	for (Unit* u : units) {
		Creature* c;
		if (c = dynamic_cast<Creature*>(u)) {
			checkOrders(c);
		}

	}

	// additing units
	for (Unit* u : additionalUnits) {
		units.push_back(u);
	}
	additionalUnits = {};

	// killing
	for (int i = 0; i < units.size(); i++) {
		if (units[i]->hp <= 0) {
			if (i == 0) {
				//start();
				status = "death";
			}
			else if (!dynamic_cast<Creature*>(units[i])) {
				Unit* unit = units[i];
				if (dynamic_cast<Dummy*>(units[i])) {
					for(int i=0; i<10; i++)
						animation("particleSmoke",
							AnimationState(units[i]->body.pos, Vector2d(0.5, 0.5), 0, {100, 100, 100}),
							AnimationState(units[i]->body.pos + Vector2d(random::floatRandom(-0.5, 0.5, 3), random::floatRandom(-0.5, 0.5, 3)), Vector2d(1, 1), 0, { 100, 100, 100, 0 }),
							random::floatRandom(0.5, 1, 3)
						);
				}
				delete unit;
				units.erase(units.begin() + i);
				i--;
			}
			else {
				sound("death", units[i]->body.pos, 100);
				Dummy* d = new Dummy();
				d->body = units[i]->body;
				d->hp = 1;
				if (dynamic_cast<Turret*>(units[i])) {
					d->type = "turret";
				}
				if (dynamic_cast<RocketLauncher*>(units[i])) {
					d->type = "rocketLauncher";
				}
				if (dynamic_cast<Robot*>(units[i])) {
					d->type = "robot";
				}
				delete units[i];
				units[i] = d;
			}
		}
	}

	// color checking
	for (int x = 0; x < field.size(); x++) {
		for (int y = 0; y < field[x].size(); y++) {
			if (!colorsActive[field[x][y].color]) {
				if (field[x][y].type != EMPTY) {
					field[x][y].type = EMPTY;
					field[x][y].spikes = 0;
					Color col = colorMatches[field[x][y].color];
					for (int i = 0; i < 5; i++) {
						Vector2d pos = Vector2d(x + 0.5, y + 0.5) + Vector2d(random::floatRandom(-0.5, 0.5, 3), random::floatRandom(-0.5, 0.5, 3));
						animation("particleFlame",
							AnimationState(pos, Vector2d(0.2, 0.2), 0, col),
							AnimationState(pos, Vector2d(0, 0), 0, col),
							random::floatRandom(0.5, 3, 3)
						);
					}
				}
				

				
			}
		}
	}

	// animations
	for (int i = 0; i < animations.size(); i++) {
		if (animations[i]->time >= animations[i]->timeFinish || animations[i]->time < animations[i]->timeStart) {
			delete animations[i];
			animations.erase(animations.begin() + i);
		}
	}
}
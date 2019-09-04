#include "System.h"
#include "geometry.h"
#include "random.h"

#include <vector>
#include <algorithm>
#include <iostream>

#include "getMilliCount.h"

using namespace random;
using namespace std;

bool System::collision(Body& body, std::pair<Vector2d, Vector2d> wall) {
	bool touch = 0;
	//calculating mid perpendicular
	Vector2d center = (wall.first + wall.second) / 2;
	std::pair<Vector2d, Vector2d> perpendicular;
	perpendicular.first = geom::rotate(wall.first - center, M_PI / 2) + center;
	perpendicular.second = geom::rotate(wall.second - center, M_PI / 2) + center;
	//length of wall
	double length = geom::distance(wall.first, wall.second);
	//pos after movement
	Vector2d posNew = body.pos + body.vel * dt;
	//checking collsion
	if (geom::distance(posNew, wall.first, wall.second) < body.r && 
		geom::distance(posNew, perpendicular.first, perpendicular.second) < length / 2 &&
		geom::distance(posNew, wall.first, wall.second) < geom::distance(body.pos, wall.first, wall.second)) {
		touch = 1;
		Vector2d velNew = geom::rotate(body.vel, -geom::angle(wall.first - wall.second));
		velNew.y *= -bounce;
		body.vel = geom::rotate(velNew, geom::angle(wall.first - wall.second));
	}
	else if (geom::distance(posNew, wall.first) < body.r) { //point collision
		touch = 1;
		Vector2d velNew = geom::rotate(body.vel, -geom::angle(wall.first - posNew));
		velNew.x *= -bounce;
		body.vel = geom::rotate(velNew, (geom::angle(wall.first - posNew)));
	}else if (geom::distance(posNew, wall.second) < body.r) {
		touch = 1;
		Vector2d velNew = geom::rotate(body.vel, -geom::angle(wall.second - posNew));
		velNew.x *= -bounce;
		body.vel = geom::rotate(velNew, (geom::angle(wall.second - posNew)));
	}
	return touch;
}

void System::collision() {
	bool touch = 0;
	//objects
	for (Unit* a : units) {
		auto neighbors = getNeighbors(a);
		for (Unit* b : neighbors) {
			Vector2d aPos = a->body.pos + a->body.vel*dt;
			Vector2d bPos = b->body.pos + b->body.vel*dt;
			if (geom::distance(aPos, bPos) < EPS || !a->collision || !b->collision)
				continue;
			if (geom::distance(a->body.pos, b->body.pos) < a->body.r + b->body.r) {
				a->body.vel += geom::direction(a->body.pos, b->body.pos)*dt*100/a->body.m;
			}
			else {
				if (geom::distance(aPos, bPos) < a->body.r + b->body.r) {
					double ang = geom::angle(a->body.pos - b->body.pos);
					Vector2d v = geom::rotate(a->body.vel, -ang);
					v.x *= bounce;
					a->body.vel = geom::rotate(v, ang);
					touch = 1;
				}
			}
			
		}
		if (touch && !dynamic_cast<Bullet*>(a) && !dynamic_cast<Explosion*>(a)) {
			if (geom::distance(a->body.vel, a->body.velPrev)>0.1)
			sound("knock", units[0]->body.pos, geom::distance(a->body.vel, a->body.velPrev));
		}
	}
	
	//walls
	for (Unit* u : units) {
		vector<pair<pair<Vector2d, Vector2d>, bool> > walls;
		walls.push_back({ {{0, 0}, {0, (double)field.size()}}, 0 });
		walls.push_back({ {{0, 0}, {(double)field.size(), 0}}, 0 });
		walls.push_back({ {{(double)field.size(), (double)field.size()}, {0, (double)field.size()}}, 0 });
		walls.push_back({ {{(double)field.size(), (double)field.size()}, {(double)field.size(), 0}}, 0 });

		int x0 = (int)(u->body.pos.x / blockSize + 1) - 1;
		int y0 = (int)(u->body.pos.y / blockSize + 1) - 1;
		for (int x = std::max(0, x0 - 1); x <= std::min((int)field.size() - 1, x0 + 1); x++) {
			for (int y = std::max(0, y0 - 1); y <= std::min((int)field[0].size() - 1, y0 + 1); y++) {
				//block verticies
				Vector2d pA(x + 0.0, y + 0.0);
				Vector2d pB(x + 1.0, y + 0.0);
				Vector2d pC(x + 1.0, y + 1.0);
				Vector2d pD(x + 0.0, y + 1.0);
				bool danger = field[x][y].spikes;
				if (field[x][y].type == WALL) {
					walls.push_back({ {pA, pB}, danger });
					walls.push_back({ {pB, pC}, danger });
					walls.push_back({ {pC, pD}, danger });
					walls.push_back({ {pD, pA}, danger });
				}
				if (field[x][y].type == CORNER_A) {
					walls.push_back({ {pA, pB}, danger });
					walls.push_back({ {pA, pD}, danger });
					walls.push_back({ {pB, pD}, danger });
				}
				if (field[x][y].type == CORNER_B) {
					walls.push_back({ {pA, pB}, danger });
					walls.push_back({ {pB, pC}, danger });
					walls.push_back({ {pA, pC}, danger });
				}
				if (field[x][y].type == CORNER_C) {
					walls.push_back({ {pB, pC}, danger });
					walls.push_back({ {pC, pD}, danger });
					walls.push_back({ {pB, pD}, danger });
				}
				if (field[x][y].type == CORNER_D) {
					walls.push_back({ {pC, pD}, danger });
					walls.push_back({ {pA, pD}, danger });
					walls.push_back({ {pA, pC}, danger });
				}
			}
		}
		bool touch = 0;
		bool dmg = 0;
		for (const auto& wall : walls) {
			if (collision(u->body, wall.first)) {
				touch = 1;
				if (wall.second)
					dmg = 1;
			}
		}
		
		if (touch && dynamic_cast<Bullet*>(u)) {
			u->hp = 0;
		}
		if (dmg && !dynamic_cast<Explosion*>(u)) {
			damage(u);
		}

		//sound
		if (touch && !dynamic_cast<Bullet*>(u) && !dynamic_cast<Explosion*>(u)) {
			if (geom::distance(u->body.vel, u->body.velPrev) > 0.1) {
				sound("knock", units[0]->body.pos, geom::distance(u->body.vel, u->body.velPrev));
				std::cout << "0\n";
			}
		}
		

	}
}
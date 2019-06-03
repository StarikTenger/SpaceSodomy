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

void System::collision() {
	bool touch = 0;
	//objects
	for (Unit* a : units) {
		auto neighbors = getNeighbors(a);
		for (Unit* b : neighbors) {
			Vector2d aPos = a->body.pos + a->body.vel*dt;
			Vector2d bPos = b->body.pos + b->body.vel*dt;
			if (distance(aPos, bPos) < EPS || !a->collision || !b->collision)
				continue;
			if (distance(a->body.pos, b->body.pos) < a->body.r + b->body.r) {
				a->body.vel += direction(a->body.pos, b->body.pos)*dt*100/a->body.m;
			}
			else {
				if (distance(aPos, bPos) < a->body.r + b->body.r) {
					double ang = angle(a->body.pos - b->body.pos);
					Vector2d v = rotate(a->body.vel, -ang);
					v.x *= bounce;
					a->body.vel = rotate(v, ang);
					touch = 1;
				}
			}
			
		}
		if (touch && !dynamic_cast<Bullet*>(a) && !dynamic_cast<Explosion*>(a)) {
			if (distance(a->body.vel, a->body.velPrev)>0.1)
			sound("knock", units[0]->body.pos, distance(a->body.vel, a->body.velPrev));
		}
	}
	//walls
	for (Unit* u : units) {
		bool touch = 0;
		auto& b = u->body;
		checkExplosions(u);
		int x = (int)(b.pos.x / blockSize + 1) - 1;
		int y = (int)(b.pos.y / blockSize + 1) - 1;
		Vector2d inCell = Vector2d(u->body.pos.x - x, u->body.pos.y - y);
		
		double r = u->body.r;
		int dmg = 0;
		//corners
		std::cout << "\n";
		for (int x1 = std::max(0, x - 1); x1 <= std::min((int)field.size()-1, x + 1); x1++) {
			for (int y1 = std::max(0, y - 1); y1 <= std::min((int)field[0].size()-1, y + 1); y1++) {
				if (field[x1][y1].type == CORNER_A || field[x1][y1].type == CORNER_B || field[x1][y1].type == CORNER_C || field[x1][y1].type == CORNER_D) {
					std::vector<Vector2d> points = { Vector2d(x1, y1), Vector2d(x1 + 1, y1), Vector2d(x1 + 1, y1 + 1), Vector2d(x1, y1 + 1) };
					std::vector<Vector2d> points1 = { Vector2d(x1, y1), Vector2d(x1 + 1, y1), Vector2d(x1 + 1, y1 + 1), Vector2d(x1, y1 + 1) };
					if (field[x1][y1].type == CORNER_A || field[x1][y1].type == CORNER_C) {
						points.erase(points.begin() + 2);
						points.erase(points.begin() + 0);
						points1.erase(points1.begin() + 3);
						points1.erase(points1.begin() + 1);
					}
					if (field[x1][y1].type == CORNER_B || field[x1][y1].type == CORNER_D) {
						points.erase(points.begin() + 3);
						points.erase(points.begin() + 1);
						points1.erase(points1.begin() + 2);
						points1.erase(points1.begin() + 0);
					}
					Vector2d pos1 = b.pos + b.vel*dt;
					std::cout << geom::distance(pos1, points[0], points[1]) << " corner\n";
					if (geom::distance(pos1, points[0], points[1]) < u->body.r && geom::distance(pos1, points1[0], points1[1]) < sqrt(2)/2) {
						double ang = geom::angle(points[0] - points[1]);
						Vector2d vel = geom::rotate(u->body.vel, ang);
						vel.x *= -bounce;
						u->body.vel = geom::rotate(vel, -ang);
						touch = 1;
						
						if (dynamic_cast<Bullet*>(u)) {
							u->hp = 0;
						}
					}
				}
			}
		}
		


		vector<Vector2d> anchors = {
			{0, r},
			{0, -r},
			{r, 0},
			{-r, 0}
		};
		for (auto& a : anchors) {
			Vector2d p = b.pos + a;
			int x = (int)(p.x / blockSize + 1) - 1;
			int y = (int)(p.y / blockSize + 1) - 1;
			int x1 = (int)((p.x + b.vel.x*dt) / blockSize + 1) - 1;
			int y1 = (int)((p.y + b.vel.y*dt) / blockSize + 1) - 1;
			if (x1 < 0 || x1 >= field.size() || y >= 0 && y < field[0].size() && field[x1][y].type == WALL) {
				if (x1 >= 0 && x1 < field.size() && y >= 0 && y < field[0].size() && field[x1][y].spikes)
					dmg = 1;
				b.vel.x *= -bounce;
				touch = 1;
				if (dynamic_cast<Bullet*>(u)) {
					u->hp = 0;
				}
			}
			if (y1 < 0 || y1 >= field[0].size() || x >= 0 && x < field.size() && field[x][y1].type == WALL) {
				if (x >= 0 && x < field.size() && y1 >= 0 && y1 < field[0].size() && field[x][y1].spikes)
					dmg = 1;
				b.vel.y *= -bounce;
				touch = 1;
				if (dynamic_cast<Bullet*>(u)) {
					u->hp = 0;
				}
			}
		}

		int x1 = (int)((b.pos.x + b.vel.x*dt) / blockSize + 1) - 1;
		int y1 = (int)((b.pos.y + b.vel.y*dt) / blockSize + 1) - 1;
		vector<Vector2d> points;
		vector<int> danger;
		if (x1 > 0 && y1 > 0) {
			if (field[x1 - 1][y1 - 1].type && field[x1 - 1][y1 - 1].type != CORNER_A) {
				points.push_back(Vector2d(x1, y1));
				danger.push_back(field[x1 - 1][y1 - 1].spikes);
			}
		}
		if (x1 < field.size() - 1 && y1 > 0) {
			if (field[x1 + 1][y1 - 1].type && field[x1 + 1][y1 - 1].type != CORNER_B) {
				points.push_back(Vector2d(x1 + 1, y1 ));
				danger.push_back(field[x1 + 1][y1 - 1].spikes);
			}
		}
		if (x1 > 0 && y1 < field[0].size() - 1) {
			if (field[x1 - 1][y1 + 1].type && field[x1 - 1][y1 + 1].type != CORNER_D) {
				points.push_back(Vector2d(x1, y1 + 1 ));
				danger.push_back(field[x1 - 1][y1 + 1].spikes);
			}
		}
		if (x1 < field.size() - 1 && y1 < field[0].size() - 1) {
			if (field[x1 + 1][y1 + 1].type && field[x1 + 1][y1 + 1].type != CORNER_C) {
				points.push_back(Vector2d(x1 + 1, y1 + 1 ));
				danger.push_back(field[x1 + 1][y1 + 1].spikes);
			}
		}
		int i = 0;
		//if(0)
		for (auto& p : points) {
			Vector2d pos = u->body.pos + u->body.vel*dt;
			if (distance(p, pos) > u->body.r)
				continue;
			if (danger[i])
				dmg = 1;
			double a = angle(p - pos);
			Vector2d velRel = rotate(u->body.vel, -a);
			velRel.x *= -bounce;
			u->body.vel = rotate(velRel, a);
			u->body.vel += direction(p, u->body.pos)*dt*(-10);
			touch = 1;
			if (dynamic_cast<Bullet*>(u)) {
				u->hp = 0;
			}
			i++;
		}
		if (touch && !dynamic_cast<Bullet*>(u) && !dynamic_cast<Explosion*>(u)) {
			if(distance(u->body.vel, u->body.velPrev)>0.1)
			sound("knock", units[0]->body.pos, distance(u->body.vel, u->body.velPrev));
		}
		if (dmg && !dynamic_cast<Explosion*>(u)) {
			damage(u);
		}
	}

}
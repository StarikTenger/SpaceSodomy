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
		for (Unit* b : units) {
			Vector2d aPos = a->body.pos + a->body.vel*dt;
			Vector2d bPos = b->body.pos + b->body.vel*dt;
			if (distance(aPos, bPos) < EPS || !a->collision || !b->collision)
				continue;
			if (distance(a->body.pos, b->body.pos) < a->body.r + b->body.r) {
				a->body.vel += direction(a->body.pos, b->body.pos)*dt * 10;
			}else
			if (distance(aPos, bPos) < a->body.r + b->body.r) {
				
				double ang = angle(a->body.pos - b->body.pos);
				Vector2d pa = rotate(a->body.vel, -ang)*a->body.m;
				Vector2d pb = rotate(b->body.vel, -ang)*b->body.m;
				Vector2d dp = direction(a->body.pos, b->body.pos)*((pa.x - pb.x) / 2);
				a->body.vel -= dp / a->body.m;
				b->body.vel += dp / b->body.m;
				touch = 1;
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
		//std::cout << x << " " << y << "\n";
		
		double r = u->body.r;
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
			if (x1 < 0 || x1 >= field.size() || y >= 0 && y < field[0].size() && field[x1][y].type != 0) {
				b.vel.x *= -bounce;
				touch = 1;
				if (dynamic_cast<Bullet*>(u)) {
					u->hp = 0;
				}
			}
			if (y1 < 0 || y1 >= field[0].size() || x >= 0 && x < field.size() && field[x][y1].type != 0) {
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
		if (x1 > 0 && y1 > 0) {
			if (field[x1 - 1][y1 - 1].type) {
				points.push_back(Vector2d(x1, y1));
			}
		}
		if (x1 < field.size() - 1 && y1 > 0) {
			if (field[x1 + 1][y1 - 1].type) {
				points.push_back(Vector2d(x1 + 1, y1 ));
			}
		}
		if (x1 > 0 && y1 < field[0].size() - 1) {
			if (field[x1 - 1][y1 + 1].type) {
				points.push_back(Vector2d(x1, y1 + 1 ));
			}
		}
		if (x1 < field.size() - 1 && y1 < field[0].size() - 1) {
			if (field[x1 + 1][y1 + 1].type) {
				points.push_back(Vector2d(x1 + 1, y1 + 1 ));
			}
		}
		for (auto& p : points) {
			Vector2d pos = u->body.pos + u->body.vel*dt;
			if (distance(p, pos) > u->body.r)
				continue;
			double a = angle(p - pos);
			Vector2d velRel = rotate(u->body.vel, -a);
			velRel.x *= -bounce;
			u->body.vel = rotate(velRel, a);
			u->body.vel += direction(p, u->body.pos)*dt*(-10);
			touch = 1;
			if (dynamic_cast<Bullet*>(u)) {
				u->hp = 0;
			}
		}
		if (touch && !dynamic_cast<Bullet*>(u) && !dynamic_cast<Explosion*>(u)) {
			if(distance(u->body.vel, u->body.velPrev)>0.1)
			sound("knock", units[0]->body.pos, distance(u->body.vel, u->body.velPrev));
		}
	}

}
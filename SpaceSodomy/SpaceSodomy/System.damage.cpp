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


void System::damage(Unit* c) {
	if (dynamic_cast<Creature*>(c) && (dynamic_cast<Creature*>(c)->immortality > 0 || dynamic_cast<Creature*>(c)->hp <= 0)) {
		return;
	}
	bool dmg = 0;
	if (dynamic_cast<Creature*>(c) && dynamic_cast<Creature*>(c)->shields) {
		dynamic_cast<Creature*>(c)->shields--;
		dynamic_cast<Creature*>(c)->immortality = 1;
		dmg = 1;
		if (dynamic_cast<Ship*>(c) && !dynamic_cast<Robot*>(c) && status != "death") { //!!!!!NEED FIX!!!!! add class for player
			
			events.push_back("damage");
		}
	}
	else {
		dmg = dynamic_cast<Creature*>(c);
		
		if (dynamic_cast<Dummy*>(c)) {
			c->hp -= dt;
		}
		else {
			c->hp -= 100;
		}
		
		if (dynamic_cast<Ship*>(c) && !dynamic_cast<Robot*>(c) && status != "death") { //!!!!!NEED FIX!!!!! add class for player
			events.push_back("damage");
		}
	}

	if (dmg && status != "death") {
		
		for (int i = 0; i < 30 * particleLevel; i++) {
			double duration = floatRandom(0.5, 1, 3);
			Vector2d pos = direction(floatRandom(0, 2 * M_PI, 3)) * floatRandom(1, 2, 3) + c->body.vel * duration;
			animation("shield",
				AnimationState(c->body.pos, Vector2d(0.2, 0.2), c->body.direction, Color(255, 255, 255)),
				AnimationState(c->body.pos + pos, Vector2d(0.2, 0.2), c->body.direction + floatRandom(0, 2 * M_PI, 3), Color(255, 255, 255, 0)),
				duration
			);
		}
	}
}
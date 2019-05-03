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
	if (dynamic_cast<Creature*>(c) && dynamic_cast<Creature*>(c)->shields) {
		dynamic_cast<Creature*>(c)->shields--;
		dynamic_cast<Creature*>(c)->immortality = 1;
		if (dynamic_cast<Ship*>(c) && !dynamic_cast<Robot*>(c) && status != "death") { //!!!!!NEED FIX!!!!! add class for player
			events.push_back("damage");
		}
		return;
	}
	
	c->hp -= 100;
	if (dynamic_cast<Ship*>(c) && !dynamic_cast<Robot*>(c) && status != "death") { //!!!!!NEED FIX!!!!! add class for player
		events.push_back("damage");
	}
}
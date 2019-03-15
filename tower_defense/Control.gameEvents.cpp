#include <sstream>
#include "Control.h"

using namespace std;

void Control::gameEvents() {
	for (int i = 0; i < sys.events.size(); i++) {
		string type;
		stringstream event;
		event << sys.events[i];
		event >> type;
		if (type == "damage") {
			audio.play("damage"); 
			//drawSys.
			sys.events.erase(sys.events.begin() + i);
		}
		else {
			double x, y, v;
			event >> x >> y >> v;
			audio.play(type, Vector2d(x, y), min(100.0, v));
			sys.events.erase(sys.events.begin() + i);
		}
	}
}
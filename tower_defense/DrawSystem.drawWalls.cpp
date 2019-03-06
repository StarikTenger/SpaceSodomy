#include "DrawSystem.h"
#include <math.h>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>


void DrawSystem::drawWalls(System& sys) {
	for (int x = 0; x < sys.field.size(); x++) {
		for (int y = 0; y < sys.field[0].size(); y++) {
			if (sys.field[x][y].type) {
				image("wall", x+0.5, y + 0.5, 1, 1, 0);
				if (x > 0 && !sys.field[x - 1][y].type)
					image("wallBorder", x + 0.5, y + 0.5, 1, 1, M_PI);
				if (x < sys.field.size()-1 && !sys.field[x + 1][y].type)
					image("wallBorder", x + 0.5, y + 0.5, 1, 1, 0);
				if (y > 0 && !sys.field[x][y - 1].type)
					image("wallBorder", x + 0.5, y + 0.5, 1, 1, -M_PI*0.5);
				if (y < sys.field.size() - 1 && !sys.field[x][y + 1].type)
					image("wallBorder", x + 0.5, y + 0.5, 1, 1, M_PI*0.5);
			}
		}
	}
}


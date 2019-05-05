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
				Color color(0, 151, 255);
				if (sys.field[x][y].type == 2) {
					double t = (sys.time - x*0.1) * 5;
					color = Color(255, 127 + sin(t) * 128, 127 + sin(t) * 128);
				}
				image("wall", x+0.5, y + 0.5, 1, 1, 0, color);
				if (x > 0 && !sys.field[x - 1][y].type)
					image("wallBorder", x + 0.5, y + 0.5, 1, 1, M_PI, color);
				if (x < sys.field.size()-1 && !sys.field[x + 1][y].type)
					image("wallBorder", x + 0.5, y + 0.5, 1, 1, 0, color);
				if (y > 0 && !sys.field[x][y - 1].type)
					image("wallBorder", x + 0.5, y + 0.5, 1, 1, -M_PI*0.5, color);
				if (y < sys.field.size() - 1 && !sys.field[x][y + 1].type)
					image("wallBorder", x + 0.5, y + 0.5, 1, 1, M_PI*0.5, color);
			}
		}
	}
}


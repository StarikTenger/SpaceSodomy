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
				Color color1 = color;
				int type = 0;
				if (sys.field[x][y].type == 2) {
					type = 1;
					double t = (sys.time - x * 0) * 5;
					color = Color(255, 0, 0);
					color1 = Color(255, 255, 255);
				}
				int up, down, left, right;
				up = down = left = right = 0;
				image("wall", x + 0.5, y + 0.5, 1, 1, 0, Color(0, 151, 255));

				if (x > 0 && !sys.field[x - 1][y].type) {
					left = 1;
				}
				if (x < sys.field.size() - 1 && !sys.field[x + 1][y].type) {
					right = 1;
				}
				if (y > 0 && !sys.field[x][y - 1].type) {
					up = 1;
				}
				if (y < sys.field.size() - 1 && !sys.field[x][y + 1].type) {
					down = 1;
				}
				if (type) {
					if (up && right)
						image("glowCornerRed", x + 1 + 0.5, y - 1 + 0.5, 1, 1, M_PI * 0, color1);
					if (down && right)
						image("glowCornerRed", x + 1 + 0.5, y + 1 + 0.5, 1, 1, M_PI * 0.5, color1);
					if (down && left)
						image("glowCornerRed", x - 1 + 0.5, y + 1 + 0.5, 1, 1, M_PI * 1, color1);
					if (up && left)
						image("glowCornerRed", x - 1 + 0.5, y - 1 + 0.5, 1, 1, M_PI * 1.5, color1);
				}
			}
		}
	}
	for (int x = 0; x < sys.field.size(); x++) {
		for (int y = 0; y < sys.field[0].size(); y++) {
			if (sys.field[x][y].type) {
				Color color(0, 151, 255);
				Color color1 = color;
				int type = 0;
				if (sys.field[x][y].type == 2) {
					type = 1;
					double t = (sys.time - x * 0) * 5;
					color = Color(255, 0, 0);
					color1 = Color(255, 255, 255);
					//color = Color(255, 127 + sin(t) * 128, 127 + sin(t) * 128);
				}
				int up, down, left, right;
				up = down = left = right = 0;
				

				if (x > 0 && !sys.field[x - 1][y].type) {
					left = 1;
					if (type)
						image("glowRed", x - 1 + 0.5, y + 0.5, 1, 1, M_PI, color1);
					image("wallBorder", x + 0.5, y + 0.5, 1, 1, M_PI, color1);
				}
				if (x < sys.field.size() - 1 && !sys.field[x + 1][y].type) {
					right = 1;
					if (type)
						image("glowRed", x + 1 + 0.5, y + 0.5, 1, 1, 0, color1);
					image("wallBorder", x + 0.5, y + 0.5, 1, 1, 0, color1);
				}
				if (y > 0 && !sys.field[x][y - 1].type) {
					up = 1;
					if (type)
						image("glowRed", x + 0.5, y - 1 + 0.5, 1, 1, -M_PI * 0.5, color1);
					image("wallBorder", x + 0.5, y + 0.5, 1, 1, -M_PI * 0.5, color1);
				}
				if (y < sys.field.size() - 1 && !sys.field[x][y + 1].type) {
					down = 1;
					if (type)
						image("glowRed", x + 0.5, y + 1 + 0.5, 1, 1, M_PI * 0.5, color1);
					image("wallBorder", x + 0.5, y + 0.5, 1, 1, M_PI*0.5, color1);
				}
			}
		}
	}
}


#include "System.h"
#include "geometry.h"
#include "random.h"

#include <iostream>

using namespace geom;
using namespace random;

int System::checkWall(Vector2d pos) {
	int x = (int)(pos.x / blockSize);
	int y = (int)(pos.y / blockSize);
	Vector2d rel = pos - Vector2d(x+0.5, y+0.5);
	if ((x < 0 || y < 0 || x >= field.size() || y >= field[0].size() || field[x][y].type == 1) || 
		rel.y < -rel.x && field[x][y].type == CORNER_A || rel.y > -rel.x && field[x][y].type == CORNER_C || 
		rel.y < rel.x && field[x][y].type == CORNER_B || rel.y > rel.x && field[x][y].type == CORNER_D
		) {
		return 1;
	}
	return 0;
}
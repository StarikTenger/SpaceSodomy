#include "System.h"
#include "geometry.h"
#include "random.h"

#include <iostream>

using namespace geom;
using namespace random;

int System::checkWall(Vector2d pos) {
	int x = (int)(pos.x / blockSize);
	int y = (int)(pos.y / blockSize);
	if (x < 0 || y < 0 || x >= field.size() || y >= field[0].size() || field[x][y].type == 1) {
		return 1;
	}
	return 0;
}
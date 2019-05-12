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

Cell& System::getCell(Vector2d pos) {
	int x = (int)(pos.x / blockSize + 1) - 1;
	int y = (int)(pos.y / blockSize + 1) - 1;
	if (x >= 0 && y >= 0 && x < field.size() && y < field[0].size()) {
		return field[x][y];
	}
	else {
		Cell c;
		c.type = 1;
		return c;
	}
}
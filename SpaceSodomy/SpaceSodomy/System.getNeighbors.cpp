#include "System.h"
#include "geometry.h"
#include "random.h"

#include <iostream>
#include <fstream>
#include <algorithm>

using namespace geom;
using namespace random;
using namespace std;

std::vector<Unit*> System::getNeighbors(const Unit* unit) {
	std::vector<Unit*> neighbors;
	int n = field.size() / chunkSize + 1; //number of chunks n^2
	int x1 = (int)(unit->body.pos.x / chunkSize);
	int y1 = (int)(unit->body.pos.y / chunkSize);
	for (int x = max(0, x1 - 1); x < min(n, x1 + 1); x++) {
		for (int y = max(0, y1 - 1); y < min(n, y1 + 1); y++) {
			for (Unit* u : chunks[x][y]) {
				neighbors.push_back(u);
			}
		}
	}
	return neighbors;
}
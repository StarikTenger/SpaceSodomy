#include "System.h"
#include "geometry.h"
#include "random.h"

#include <iostream>
#include <fstream>

using namespace geom;
using namespace random;
using namespace std;

void System::fillChunks() {
	chunks = {};
	int n = field.size() / chunkSize + 1; //number of chunks n^2
	for (int x = 0; x < n; x++) {
		chunks.push_back({});
		for (int y = 0; y < n; y++) {
			chunks[x].push_back({}); 
		}
	}
	for (Unit* u : units) {
		int x = (int)(u->body.pos.x / chunkSize);
		int y = (int)(u->body.pos.y / chunkSize);
		//cout << x << " " << y <<  " " << chunks.size() << "\n";
		chunks[x][y].push_back(u);
	}
}
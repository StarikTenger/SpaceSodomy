#include "System.h"
#include "geometry.h"
#include "random.h"

#include <iostream>
#include <fstream>

using namespace geom;
using namespace random;
using namespace std;

System::System(){
} 

Ship* System::getShip(int n) {
	if (n >= units.size())
		return nullptr;
	return dynamic_cast<Ship*>(units[n]);
}
Bullet* System::getBullet(int n) {
	return dynamic_cast<Bullet*>(units[n]);
}

System::System(int width, int height) {
	units.push_back(new Ship(
		Engine(1.6, 1.6, 8),
		Gun(1, 10)
	));
	units.push_back(new Turret(
		Gun(1, 10),
		1
	));

	
	units[0]->body.pos = { 10, 10 };
	units[1]->body.pos = { 15, 15 };

	getShip(0)->team = "good";
	for (int x = 0; x < width; x++) {
		field.push_back({});
		for (int y = 0; y < height; y++) {
			field[x].push_back({});
			if (x==0 || y==0 ) {
				field[x][y].type = 1;
			}
		}
	}
}
System::~System(){
}

System::System(string path) {
	ifstream file(path);
	int width, height;
	file >> width >> height;
	for (int x = 0; x < width; x++) {
		field.push_back(vector<Cell>(height));
	}
	for (int x = 0; x < width; x++) {
		//field.push_back({});
		for (int y = 0; y < height; y++) {
			Cell c;
			file >> c.type;
			field[y][x] = c;
		}
	}
	while (file) {
		string command;
		file >> command;
		if (command == "END") {
			break;
		}
		if (command == "SPAWNPOINT") {
			std::cout << "spawn\n";
			Ship* ship = new Ship(
				Engine(4, 4, 4),
				Gun(0.5, 15));
			file >> ship->body.pos.x >> ship->body.pos.y;
			ship->body.pos.x += 0.5*blockSize;
			ship->body.pos.y += 0.5*blockSize;
			ship->body.r = 0.24;
			ship->shields = 3;
			units.push_back(ship);
		} else
		if (command == "TURRET") {
			Turret* turret = new Turret();
			turret->team = "enemy";
			turret->body.r = 0.4;
			turret->body.m = 2;
			while (1) {
				std::string characteristic;
				file >> characteristic;
				if (characteristic == "END") {
					cout << "END\n";
					break;
				} 
				else if (characteristic == "POS") {
					file >> turret->body.pos.x >> turret->body.pos.y;
					turret->body.pos.x += 0.5*blockSize;
					turret->body.pos.y += 0.5*blockSize;
				}
				else if (characteristic == "DIR") {
					file >> turret->body.direction;
				}
				else if (characteristic == "W") {
					file >> turret->body.w;
				}
				else if (characteristic == "CD") {
					file >> turret->gun.cooldownTime;
				}
				else if (characteristic == "BV") {
					file >> turret->gun.bulletVelocity;
				}
				else if (characteristic == "M") {
					file >> turret->body.m;
				}
				else if (characteristic == "R") {
					file >> turret->body.r;
				} else {
					cout << characteristic << "\n";
					//return;
				}
		}
			units.push_back(turret);
		} else
		if (command == "EXIT") {
			Exit* exit = new Exit();
			file >> exit->body.pos.x >> exit->body.pos.y;
			exit->body.pos.x += 0.5*blockSize;
			exit->body.pos.y += 0.5*blockSize;
			units.push_back(exit);
		}
	}
}

void System::sound(string name, Vector2d pos, double volume) {
	pos -= units[0]->body.pos;
	pos = rotate(pos, -units[0]->body.direction);
	//cout << pos.x << " " << pos.y << "\n";
	events.push_back(name + " " + to_string(pos.x) + " " + to_string(pos.y) + " " + to_string(volume));
}
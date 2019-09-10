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
		for (int y = 0; y < height; y++) {
			Cell c;
			std::string type;
			file >> type;
			if (type == "0") {
				c.type = 0;
			}
			else if (type == "0") {
				c.type = EMPTY;
			}
			else if (type == "1") {
				c.type = WALL;
			}
			else if (type == "2") {
				c.type = WALL;
				c.spikes = 1;
			}
			else if (type == "A" || type == "a") {
				c.type = CORNER_A;
			}
			else if (type == "B" || type == "b") {
				c.type = CORNER_B;
			}
			else if (type == "C" || type == "c") {
				c.type = CORNER_C;
			}
			else if (type == "D" || type == "d") {
				c.type = CORNER_D;
			}
			field[y][x] = c;
		}
	}
	while (file) {
		string command;
		file >> command;
		if (command == "END") {
			break;
		}
		if (command == "BACKGROUND") {
			file >> background;
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
			ship->shields = 1;
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
		} else
		if (command == "BONUS") {
			Bonus* bonus = new Bonus();
			while (1) {
				std::string characteristic;
				file >> characteristic;
				if (characteristic == "END") {
					cout << "END\n";
					break;
				}
				else if (characteristic == "POS") {
					file >> bonus->body.pos.x >> bonus->body.pos.y;
					bonus->body.pos.x += 0.5*blockSize;
					bonus->body.pos.y += 0.5*blockSize;
				}
				else if (characteristic == "DIR") {
					file >> bonus->body.direction;
				}
				else if (characteristic == "W") {
					file >> bonus->body.w;
				}
				else if (characteristic == "M") {
					file >> bonus->body.m;
				}
				else if (characteristic == "R") {
					file >> bonus->body.r;
				}
				else if (characteristic == "TYPE") {
					file >> bonus->type;
				}
				else {
					cout << characteristic << "\n";
					//return;
				}
			}
			units.push_back(bonus);
		} else
		if (command == "ROCKET_LAUNCHER") {
			RocketLauncher* rocketLauncher = new RocketLauncher();
			rocketLauncher->body.m = 10000;
			rocketLauncher->shields = 10;
			rocketLauncher->team = "enemy";
			while (1) {
				std::string characteristic;
				file >> characteristic;
				if (characteristic == "END") {
					cout << "END\n";
					break;
				}
				else if (characteristic == "POS") {
					file >> rocketLauncher->body.pos.x >> rocketLauncher->body.pos.y;
					rocketLauncher->body.pos.x += 0.5*blockSize;
					rocketLauncher->body.pos.y += 0.5*blockSize;
				}
				else if (characteristic == "DIR") {
					file >> rocketLauncher->body.direction;
				}
				else if (characteristic == "W") {
					file >> rocketLauncher->body.w;
				}
				else if (characteristic == "M") {
					file >> rocketLauncher->body.m;
				}
				else if (characteristic == "R") {
					file >> rocketLauncher->body.r;
				}
				else if (characteristic == "DIRECTIONS") {
					file >> rocketLauncher->gun.directions;
				}
				else if (characteristic == "CD") {
					file >> rocketLauncher->gun.cooldownTime;
				}
				else if (characteristic == "BV") {
					file >> rocketLauncher->gun.bulletVelocity;
				}
				else {
					cout << characteristic << "\n";
					//return;
				}
			}
			units.push_back(rocketLauncher);
		} else
		if (command == "LASER_CARRIER") {
			LaserCarrier* laserCarrier = new LaserCarrier();
			laserCarrier->body.m = 10000;
			laserCarrier->shields = 100;
			laserCarrier->team = "enemy";
			while (1) {
				std::string characteristic;
				file >> characteristic;
				if (characteristic == "END") {
					cout << "END\n";
					break;
				}
				else if (characteristic == "POS") {
					file >> laserCarrier->body.pos.x >> laserCarrier->body.pos.y;
					laserCarrier->body.pos.x += 0.5*blockSize;
					laserCarrier->body.pos.y += 0.5*blockSize;
				}
				else if (characteristic == "DIR") {
					file >> laserCarrier->body.direction;
				}
				else if (characteristic == "W") {
					file >> laserCarrier->body.w;
				}
				else if (characteristic == "M") {
					file >> laserCarrier->body.m;
				}
				else if (characteristic == "R") {
					file >> laserCarrier->body.r;
				}
				else if (characteristic == "DIRECTIONS") {
					int d;
					file >> d;
					auto directions = geom::angleDistribution(0, 2*M_PI, d);
					for (auto a : directions) {
						Laser laser;
						laser.direction = a;
						laserCarrier->lasers.push_back(laser);
					}
				}
				else {
					cout << characteristic << "\n";
					//return;
				}
			}
			units.push_back(laserCarrier);
		} else
		if (command == "ROBOT") {
			Robot* robot = new Robot();
			robot->body.m = 1;
			robot->shields = 1;
			robot->team = "enemy";
			while (1) {
				std::string characteristic;
				file >> characteristic;
				if (characteristic == "END") {
					cout << "END\n";
					break;
				}
				else if (characteristic == "POS") {
					file >> robot->body.pos.x >> robot->body.pos.y;
					robot->body.pos.x += 0.5*blockSize;
					robot->body.pos.y += 0.5*blockSize;
				}
				else if (characteristic == "DIR") {
					file >> robot->body.direction;
				}
				else if (characteristic == "W") {
					file >> robot->body.w;
				}
				else if (characteristic == "CD") {
					file >> robot->gun.cooldownTime;
				}
				else if (characteristic == "BV") {
					file >> robot->gun.bulletVelocity;
				}
				else if (characteristic == "M") {
					file >> robot->body.m;
				}
				else if (characteristic == "R") {
					file >> robot->body.r;
				}
				else if (characteristic == "MF") {
					file >> robot->engine.mainForce;
					robot->engine.backForce = robot->engine.mainForce;
				}
				else if (characteristic == "TF") {
					file >> robot->engine.turnForce;
				}
				else if (characteristic == "SHIELDS") {
					file >> robot->shields;
				}
				else if (characteristic == "TEAM") {
					file >> robot->team;
				}
				
			}
			units.push_back(robot);
		} else
		if (command == "GENERATOR") {
			Generator* generator = new Generator();
			while (1) {
				std::string characteristic;
				file >> characteristic;
				if (characteristic == "END") {
					cout << "END\n";
					break;
				}
				else if (characteristic == "POS") {
					file >> generator->body.pos.x >> generator->body.pos.y;
					generator->body.pos.x += 0.5*blockSize;
					generator->body.pos.y += 0.5*blockSize;
				}
				else if (characteristic == "DIR") {
					file >> generator->body.direction;
				}
				else if (characteristic == "W") {
					file >> generator->body.w;
				}
				else if (characteristic == "M") {
					file >> generator->body.m;
				}
				else if (characteristic == "R") {
					file >> generator->body.r;
				}
				else if (characteristic == "COLOR") {
					file >> generator->color;
				}
				else {
					cout << characteristic << "\n";
				}
			}
			units.push_back(generator);
		} else
		if (command == "COLORS") {
			for (int x = 0; x < width; x++) {
				for (int y = 0; y < height; y++) {
					file >> field[y][x].color; 
				}
			}
		}
	
	}
}

void System::loadUnitCharacteristics(Unit* c, ifstream file) {

}

void System::sound(string name, Vector2d pos, double volume) {
	pos -= units[0]->body.pos;
	pos = rotate(pos, -units[0]->body.direction);
	//cout << pos.x << " " << pos.y << "\n";
	events.push_back(name + " " + to_string(pos.x) + " " + to_string(pos.y) + " " + to_string(volume));
}

void System::animation(std::string name, AnimationState stateStart, AnimationState stateFinish, double duration) {
	if(geom::distance(units[0]->body.pos, stateStart.pos)<10)
		animations.push_back(
			new Animation(name, stateStart, stateFinish, time, time + duration)
		);
}

void System::particleSplash(std::string name, Vector2d pos, Vector2d box, double direction, double width, int n, double r, double duration) {
	std::vector<double> directions = geom::angleDistribution(direction, width, n);
	for (double d : directions) {
		Vector2d pos1 = geom::direction(direction) * r;
		animation(name,
			AnimationState(pos, box, d, Color(255, 255, 255)),
			AnimationState(pos + pos1, box, d, Color(255, 255, 255, 0)),
			duration
		);
	}
}

bool System::checkTime(double t) {
	return abs(int(time / t) - time / t) <= dt || abs(int(time / t + 1) - time / t) <= dt;
}
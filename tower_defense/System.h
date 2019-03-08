#pragma once
#include <vector>
#include "Vector2d.h"
#include <string>
#include "Cell.h"
#include "Unit.h"
#include "Ship.h"
#include "Bullet.h"
#include "Turret.h"
#include "Explosion.h"
#include "Exit.h"
#include "Dummy.h"
#include "Bonus.h"

class System{
public:

	double dt = 0.005;
	double blockSize = 1;
	double time = 0;
	std::vector<std::vector<Cell> > field;
	std::vector<std::vector<std::vector<Unit*> > > chunks;
	std::vector<Unit*> units;
	std::vector<Unit*> additionalUnits;
	std::vector<std::string> events;
	double bounce = 0.5;
	std::string status = "running";
	int level = 0;

	System();
	System(int width, int height);
	System(std::string path);
	~System();

	void step();
	Ship* getShip(int n);
	Bullet* getBullet(int n);
private:
	double chunkSize = 3;
	void fillChunks();
	void sound(std::string name, Vector2d pos, double volume);
	void collision();
	void start();
	void think(Creature* c);
	void checkOrders(Creature* c);
	void checkExplosions(Unit* c);
	int checkWall(Vector2d pos);
	std::vector<Unit*> getNeighbors(const Unit* unit);
};


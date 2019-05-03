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
#include "RocketLauncher.h"
#include "LaserCarrier.h"
#include "Robot.h"

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
	double wetFrictionK = 1;
	std::string status = "running";
	int level = 0;

	System();
	System(int width, int height);
	System(std::string path);
	~System();

	void step();
	Ship* getShip(int n);
	Bullet* getBullet(int n);
	bool checkAbility(Unit* shooter, Unit* target, double threshold);
private:
	double chunkSize = 3;
	void fillChunks();
	void sound(std::string name, Vector2d pos, double volume);
	void collision();
	void start();
	void think(Creature* c);
	void think(Turret* t);
	void think(Robot* r);
	void checkOrders(Creature* c);
	void checkExplosions(Unit* c);
	void damage(Unit* c);
	int checkWall(Vector2d pos);
	Cell& getCell(Vector2d pos);
	std::vector<Unit*> getNeighbors(const Unit* unit);
};
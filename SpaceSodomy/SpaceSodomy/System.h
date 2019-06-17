#pragma once
#include <vector>
#include <fstream>
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
#include "Generator.h"
#include "Bonus.h"
#include "RocketLauncher.h"
#include "LaserCarrier.h"
#include "Robot.h"
#include "Animation.h"

class System{
public:

	double dt = 0.005;
	double blockSize = 1;
	double time = 0;
	std::vector<std::vector<Cell> > field;
	std::vector<std::vector<std::vector<Unit*> > > chunks;
	std::vector<Unit*> units;
	std::vector<Unit*> additionalUnits;
	std::vector<Animation*> animations;
	std::vector<std::string> events;
	std::vector<int> colorsActive = {0, 0, 0, 0, 0, 0, 0};
	std::vector<Color> colorMatches = {
		Color(0, 151, 255),
		Color(50, 255, 81),
		Color(255, 238, 61),
		Color(200, 61, 255),
		Color(255, 61, 141),
		Color(255, 158, 61)
	};

	double bounce = 0.5;
	double wetFrictionK = 1;
	std::string status = "running";
	int level = 0;
	double particleLevel = 1;
	double particlePeriod = 0.01;

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
	bool checkTime(double t);
	void fillChunks();
	void sound(std::string name, Vector2d pos, double volume);
	void animation(std::string name, AnimationState stateStart, AnimationState stateFinish, double duration);
	void particleSplash(std::string name, Vector2d pos, Vector2d box, double direction, double width, int n, double r, double duration);
	void collision();
	void start();
	void think(Creature* c);
	void think(Turret* t);
	void think(Robot* r);
	void checkOrders(Creature* c);
	void checkExplosions(Unit* c);
	void damage(Unit* c);
	void loadUnitCharacteristics(Unit* c, std::ifstream file);
	int checkWall(Vector2d pos);
	Cell& getCell(Vector2d pos);
	std::vector<Unit*> getNeighbors(const Unit* unit);
};
#pragma once
#include <vector>
#include <string>
#include "Cell.h"
#include "Unit.h"
#include "Body.h"
#include "Bonus.h"
#include "Creature.h"
#include "Exit.h"
#include "LaserCarrier.h"
#include "Robot.h"
#include "Ship.h"
#include "Turret.h"
#include "RocketLauncher.h"




class System {
public:
	std::vector<std::vector<Cell> > field;
	std::vector<Unit*> units;
	double blockSize = 1;
	System();
	System(std::string path);
	~System();
	void save(std::string path);
};

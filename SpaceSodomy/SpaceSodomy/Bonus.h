#pragma once
#include "Unit.h"

class Bonus :public Unit {
public:
	std::string type = "shield";
	Bonus();
	~Bonus();
};


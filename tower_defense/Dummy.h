#pragma once
#include "Unit.h"
#include <string>
class Dummy : public Unit {
public:
	std::string type;
	Dummy();
	~Dummy();
};


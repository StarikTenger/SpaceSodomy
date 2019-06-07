#pragma once
#include <string>
#include "geometry.h"

class Button {
public:
	Vector2d pos;
	Vector2d box;
	std::string text;
	double value;
	double step = 0.1;

	Button();
	~Button();
};


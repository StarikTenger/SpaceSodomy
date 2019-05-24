#pragma once
#include <vector>
#include "MenuButton.h"
#include "Mouse.h"
class Menu {
public:
	std::vector<MenuButton*> buttons;
	std::vector<std::string> events;
	int level = 0;
	Vector2d scale;
	Mouse* mouse;
	Menu();
	~Menu();
	void step();
};


#pragma once
#include <ctime>
#include <algorithm>
#include <fstream>
#include <SFML/Graphics.hpp>

#include "keys.h"
#include "DrawSystem.h"
#include "random.h"
#include "System.h"
#include "geometry.h"
#include "Mouse.h"
#include "Gamepad.h"

class Control{
public:
	System sys;
	DrawSystem drawSys;
	Audio audio;

	Mouse mouse;
	Gamepad joystick;

	std::vector<bool> keys = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	std::vector<std::vector<int> > keyMatches;

	int level = 0;
	std::vector<std::string> levels;



	//time
	int dt = 20;
	int timePrev = 0;
	bool stop = 0;
	int iteration = 0;

	int frames = 0;
	int timePrevFrame = 0;
	int fps = 0;

	Control();
	~Control();

	void events();
	void gameEvents();
	void step();
	Vector2d getCursorPos();

};


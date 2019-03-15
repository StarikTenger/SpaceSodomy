#include "Control.h"
#include "getMilliCount.h"
#include <iostream>
#include <fstream>

Control::Control() {
	std::ifstream file("levels.l");
	while (file) {
		std::string path;
		file >> path;
		if (path == "END") {
			break;
		}
		levels.push_back(path);
	}
	sys = {levels[0]};
}


Control::~Control() {

}

Vector2d Control::getCursorPos() {
	return geom::rotate((mouse.pos - drawSys.cam.border/2) / drawSys.cam.scale, drawSys.cam.angle);
}

void Control::step() {
	int timeMs = getMilliCount();
	if (timeMs - timePrev > dt) {
		timePrev = timeMs;
		frames++;
		if (timeMs - timePrevFrame > 1000) {
			//std::cout << frames << "\n";
			frames = 0;
			timePrevFrame = timeMs;
		}
		//std::cout << getMilliCount() << "\n";
		sys.level = level;

		
		events();
		drawSys.mouse = mouse;
		for(int i = 0; i < dt/sys.dt/1000; i++)
			sys.step();
		double camVel = 0.005;
		sys.getShip(0)->orders = {};

		drawSys.cam.pos = sys.getShip(0)->body.pos + getCursorPos()*0;
		drawSys.cam.angle = sys.getShip(0)->body.direction+M_PI/2;
		if (keys[A]) {
			sys.getShip(0)->orders.left = 1;
		}
		if (keys[D]) {
			sys.getShip(0)->orders.right = 1;
		}
		if (keys[W]) {
			sys.getShip(0)->orders.forward = 1;
		}
		if (keys[S]) {
			sys.getShip(0)->orders.backward = 1;
		}
		if (keys[SPACE]) {
			sys.getShip(0)->orders.shoot = 1;
		}
		if (keys[Q]) {
			drawSys.cam.scale /= pow(drawSys.cam.scaleVel, 1 / (double)dt);
		}
		if (keys[E]) {
			drawSys.cam.scale *= pow(drawSys.cam.scaleVel, 1 / (double)dt);
		}
		if (sys.status == "restart") {
			sys = System(levels[level]);
		}
		if (sys.status == "next level") {
			level++;
			level = level % levels.size();
			std::cout << level << " " << levels.size() << "\n";
			sys = System(levels[level]);
		}

		drawSys.system = &sys;
		drawSys.draw();
		gameEvents();
	}

	iteration++;
}

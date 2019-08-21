#include "Control.h"
#include "getMilliCount.h"
#include <iostream>
#include <fstream>
#include <sstream>

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
	
	for (int i = 0; i < 50; i++) {
		keyMatches.push_back({});
	}
	std::ifstream keys("keys.conf");
	while (keys) {
		std::string key;
		keys >> key;
		while (keys) {
			std::string key1; 
			keys >> key1;
			if (key1 == "END")
				break;
			keyMatches[getKey(key1)].push_back(getKey(key));
		}
	}
	loadConfig();
	level = levelMax;
	sys = { levels[levelMax%levels.size()] };
}

Control::~Control() {

}

Vector2d Control::getCursorPos() {
	return geom::rotate((mouse.pos - drawSys.cam.border/2) / drawSys.cam.scale, drawSys.cam.angle);
}

void Control::loadConfig() {
	std::ifstream file(configParametersAdress);
	while (file) {
		std::string command;
		file >> command;
		if (command == "END") {
			break;
		} else 
		if (command == "LEVEL") {
			file >> levelMax;
		}
		if (command == "ENABLE_JOYSTICK") {
			enableJoystick = 1;
		}
	}
	file.close();
}


void Control::saveConfig() {
	std::ofstream file(configParametersAdress);
	std::string str = "";
	str += "LEVEL " + std::to_string(levelMax) + "\n";
	str += "END";
	file << str;
	file.close();
}

void Control::step() {
	int timeMs = getMilliCount();
	if (timeMs - timePrev > dt) {
		timePrev = timeMs;
		frames++;
		
		if (timeMs - timePrevFrame > 1000) {
			fps = frames;
			frames = 0;
			timePrevFrame = timeMs;
		}
		sys.level = level;

		
		events();
		drawSys.mouse = mouse;
		double camVel = 0.005;
		
		drawSys.cam.pos = sys.getShip(0)->body.pos + getCursorPos()*0;
		drawSys.cam.angle = sys.getShip(0)->body.direction+M_PI/2;
		sys.getShip(0)->orders = {};
		if (sys.status != "death") {
			double threshold = 50;
			if (keys[MOVE_LEFT] || joystick.pos.x < -threshold && enableJoystick) {
				sys.getShip(0)->orders.left = 1;
			}
			if (keys[MOVE_RIGHT] || joystick.pos.x > threshold && enableJoystick) {
				sys.getShip(0)->orders.right = 1;
			}
			if (keys[MOVE_FORWARD] || joystick.pos.y < -threshold && enableJoystick) {
				sys.getShip(0)->orders.forward = 1;
			}
			if (keys[MOVE_BACKWARD] || joystick.pos.y > threshold && enableJoystick) {
				sys.getShip(0)->orders.backward = 1;
			}
			if (keys[TURN_LEFT] || joystick.leftUp || joystick.leftDown && enableJoystick) {
				sys.getShip(0)->orders.turnLeft = 1;
			}
			if (keys[TURN_RIGHT] || joystick.rightUp || joystick.rightDown && enableJoystick) {
				sys.getShip(0)->orders.turnRight = 1;
			}
			if (keys[STABILIZE_ROTATION]) {
				sys.getShip(0)->orders.stabilizeRotation = 1;
			}
			if (keys[SHOOT] || joystick.button4 && enableJoystick) {
				sys.getShip(0)->orders.shoot = 1;
			}
			if (keys[ZOOM_OUT] || joystick.button3 && enableJoystick) {
				drawSys.cam.scale /= pow(drawSys.cam.scaleVel, 1 / (double)dt);
			}
			if (keys[ZOOM_IN] || joystick.button2 && enableJoystick) {
				drawSys.cam.scale *= pow(drawSys.cam.scaleVel, 1 / (double)dt);
			}
		}
		if (keys[RESTART] || joystick.button1 && enableJoystick) {
			sys.status = "restart";
		}
		if (keys[EXIT] && !keysPrev[EXIT]) {
			if (mode == MENU)
				mode = GAME;
			else
				mode = MENU;
		}
		
		if (mode == GAME) {
			if (sys.status == "restart") {
				sys = System(levels[level%levels.size()]);
				std::cout << level << " restart\n";
			}
			if (sys.status == "next level") {
				level++;
				level = level % levels.size();
				std::cout << level << " " << levels.size() << "\n";
				sys = System(levels[level]);
				if (level > levelMax) {
					levelMax = level;
				}
				saveConfig();
			}

			for (int i = 0; i < dt / sys.dt / 1000; i++)
				sys.step();

			drawSys.system = &sys;
			drawSys.draw();

			sf::Text text;
			text.setFont(drawSys.font);
			text.setString(std::to_string(fps));
			text.setCharacterSize(32);
			drawSys.window->draw(text);
			drawSys.window->display();

			gameEvents();
		}
		if (mode == MENU) {
			drawSys.menu = &menu;
			menu.mouse = &mouse;
			menu.level = levelMax;
			menu.step();
			menu.scale = Vector2d(drawSys.w, drawSys.h);
			drawSys.drawMenu();
			drawSys.window->display();

			for (int i = 0; i < menu.events.size(); i++) {
				std::stringstream ss;
				ss << menu.events[i];
				std::string command;
				ss >> command;
				if (command == "play") {
					//level = levelMax;
					mode = GAME;
					std::cout << level << " "<< levelMax <<  " play\n";
					continue;
				}
				if (command == "level") {
					int l;
					ss >> l;
					level = l%levels.size();
					sys = System(levels[level]);
					mode = GAME;
				}
			}
			menu.events = {};
		}
	}

	iteration++;
}

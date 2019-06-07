#include "DrawSystem.h"
#include <math.h>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <sstream>

#include <iostream>



DrawSystem::DrawSystem(){
	window = new sf::RenderWindow(sf::VideoMode(600, 600), "Level Editor");
	loadTextures();
}

DrawSystem::~DrawSystem(){}

void DrawSystem::drawScene() {
	w = window->getSize().x;
	h = window->getSize().y;
	sf::View view(sf::FloatRect(
		sf::Vector2f(cam.pos.x - w * 1 / cam.scale / 2, cam.pos.y - h * 1 / cam.scale / 2),
		sf::Vector2f(w * 1 / cam.scale, h * 1 / cam.scale)
	));
	view.setRotation(0);
	window->setView(view);
	
	image("backwall",
		(system->field.size())*blockSize / 2,
		(system->field.size())*blockSize / 2,
		(system->field.size())*blockSize,
		(system->field.size())*blockSize, 0);



	drawWalls();

	for (auto u : system->units) {
		drawShip(dynamic_cast<Ship*>(u));
		drawTurret(dynamic_cast<Turret*>(u));
		drawExit(dynamic_cast<Exit*>(u));
		drawBonus(dynamic_cast<Bonus*>(u));
		drawRocketLauncher(dynamic_cast<RocketLauncher*>(u));
		drawLaserCarrierBase(dynamic_cast<LaserCarrier*>(u));
		drawRobot(dynamic_cast<Robot*>(u));
	}
}

void DrawSystem::drawInterface() {
	window->setView(sf::View(sf::FloatRect(0, 0, w, h)));

	fillRect(menu->box.x*h/2, menu->box.y*h/2, menu->box.x*h, menu->box.y*h, Color(50, 50, 60, 250));
	for (Button* b : menu->buttons) {
		fillRect(b->pos.x*h, b->pos.y*h, b->box.x*h, b->box.y*h, Color(180, 180, 180));
		std::stringstream ss;
		ss << b->value;
		std::string str;
		ss >> str;
		text(b->text, b->pos.x*h, b->pos.y*h - 0.01*h, 0.06*h, Color(70, 70, 70));
		text(str, b->pos.x*h, b->pos.y*h + 0.04*h, 0.04*h, Color(70, 70, 70));
	}
}

void DrawSystem::draw() {
	cam.border = Vector2d(window->getSize().x, window->getSize().y);
	System& sys = *system;
	window->clear();
	drawScene();	
	drawInterface();

}

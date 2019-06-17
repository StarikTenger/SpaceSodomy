#include "DrawSystem.h"
#include <math.h>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>



DrawSystem::DrawSystem(){
	window = new sf::RenderWindow(sf::VideoMode(600, 600), "Space Sodomy Eternal");
	loadTextures();
}

DrawSystem::~DrawSystem(){}

void DrawSystem::drawScene() {
	System& sys = *system;
	sf::Texture tex;
	w = window->getSize().x;
	h = window->getSize().y;

	image("background",
		w / 2,
		h / 2,
		w * 1.5,
		w * 1.5, -sys.units[0]->body.direction
	);


	sf::View view(sf::FloatRect(
		sf::Vector2f(sys.units[0]->body.pos.x - w * 1 / cam.scale / 2, sys.units[0]->body.pos.y - h * 1 / cam.scale / 2),
		sf::Vector2f(w * 1 / cam.scale, h * 1 / cam.scale)
	));
	view.setRotation((sys.units[0]->body.direction * 180 / M_PI) + 90);
	window->setView(view);


	image("backwall1",
		(sys.field.size())*blockSize / 2,
		(sys.field.size())*blockSize / 2,
		(sys.field.size())*blockSize * 2 * 2,
		(sys.field.size())*blockSize * 2 * 2
		, 0,
		1.0 / 2
	);
	image("backwall",
		(sys.field.size())*blockSize / 2,
		(sys.field.size())*blockSize / 2,
		(sys.field.size())*blockSize,
		(sys.field.size())*blockSize, 0);
	//draw animations
	for (auto a : sys.animations) {
		a->time = sys.time;
		a->setState();
		image(a->img, a->state.pos.x, a->state.pos.y, a->state.box.x, a->state.box.y, a->state.direction, a->state.color);
	}
	for (auto u : sys.units) {
		drawTarget(dynamic_cast<Unit*>(u));
		drawExplosion(dynamic_cast<Explosion*>(u));
		drawShip(dynamic_cast<Ship*>(u));
		drawBullet(dynamic_cast<Bullet*>(u));
		drawTurret(dynamic_cast<Turret*>(u));
		drawExit(dynamic_cast<Exit*>(u));
		drawDummy(dynamic_cast<Dummy*>(u));
		drawBonus(dynamic_cast<Bonus*>(u));
		drawRocketLauncher(dynamic_cast<RocketLauncher*>(u));
		drawLaserCarrier(dynamic_cast<LaserCarrier*>(u));
		drawRobot(dynamic_cast<Robot*>(u));
		drawGenerator(dynamic_cast<Generator*>(u));
	}
	for (auto u : sys.units) {
		drawLaserCarrierBase(dynamic_cast<LaserCarrier*>(u));
	}
	//draw walls
	drawWalls(sys);
	//draw animations
	for (auto a : sys.animations) {
		if (a->img != "particleLaser")
			continue;
		a->time = sys.time;
		a->setState();
		image(a->img, a->state.pos.x, a->state.pos.y, a->state.box.x, a->state.box.y, a->state.direction, a->state.color);
	}
}

void DrawSystem::drawInterface() {
	System& sys = *system;
	//draw interface//////////////////////////////////////////////////////////////////////////////////////////////////////
	//draw shields
	{
		double size = h / 7;
		double d = size / 2;
		window->setView(sf::View(sf::FloatRect(0, 0, w, h)));
		for (int i = 0; i < dynamic_cast<Creature*>(sys.units[0])->shields; i++) {

			double k = (sys.time + i)*0.1;
			double v = 0.5;
			double r = sin(k * 12) * 0;
			double g = sin(k * 11)*v;
			double b = sin(k * 13) * 0;
			image("shield", size*0.6, h - size * 0.6 - dynamic_cast<Creature*>(sys.units[0])->shields *d + i * d + d, size, size, 0,
				Color(abs(r * 255), 128 + g * 127, abs(b * 255))
			);
		}
		for (auto& e : sys.events) {
			if (e == "damage") {
				animation(
					"shield",
					AnimationState({ size*0.6, h - size * 0.6 - dynamic_cast<Creature*>(sys.units[0])->shields *d + d }, { size, size }, 0, Color(0, 255, 0, 255)),
					AnimationState({ size*0.6, 0 }, { size, size }, 0, Color(0, 0, 0, 0)),
					1
				);
				damageScreen();
			}
		}
	}
	//events


	//anime
	for (int i = 0; i < animations.size(); i++) {
		if (animations[i].time >= animations[i].timeFinish || animations[i].time < animations[i].timeStart) {
			animations.erase(animations.begin() + i);
		}
	}
	for (auto& a : animations) {
		a.time = sys.time;
		a.setState();
		image(a.img, a.state.pos.x, a.state.pos.y, a.state.box.x, a.state.box.y, a.state.direction, a.state.color);
	}
	if (sys.status == "death") {
		image("death", w / 2, h / 2, w, h, 0);
	}
}

void DrawSystem::draw() {
	System& sys = *system;
	window->clear();

	drawScene();
	
	drawInterface();
}

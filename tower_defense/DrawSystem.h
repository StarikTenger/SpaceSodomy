#pragma once
#include <map>
#include <SFML/Graphics.hpp>
#include "Color.h"
#include "System.h"
#include "Camera.h"
#include "Mouse.h"
#include "Animation.h"
#include "Audio.h"

class DrawSystem{
public:
	sf::RenderWindow* window;
	Camera cam;
	System* system;
	bool fullscreenMode = 0;
	DrawSystem();
	~DrawSystem();
	Mouse mouse;
	double w, h;
	double blockSize = 1;

	void draw();
	//decoration
	void damageScreen();
private:
	//basics
	std::vector<Animation> animations;
	void animation(std::string img, AnimationState p1, AnimationState p2, double time);
	void fillRect(double x, double y, double width, double height, Color color);
	void strokeRect(double x, double y, double width, double height, Color color);
	void image(std::string name, double x, double y, double width, double height, double angle);
	void image(std::string name, double x, double y, double width, double height, double angle, Color color);
	void image(std::string name, double x, double y, double width, double height, double angle, double d);
	//objects
	void drawWalls(System& sys);
	void drawTarget(Creature* s);
	void drawShip(Ship* s);
	void drawBullet(Bullet* s);
	void drawTurret(Turret* s);
	void drawExplosion(Explosion* s);
	void drawExit(Exit* s);
	void drawDummy(Dummy* s);
	void drawBonus(Bonus* s);
	void drawRocketLauncher(RocketLauncher* s);
	//load
	std::map<std::string, sf::Texture*> textures;
	void loadWalls(std::string name, std::string img, System& sys);
	void loadTexture(std::string name, std::string path);
	void loadTextures();
};


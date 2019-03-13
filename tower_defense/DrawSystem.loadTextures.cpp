#include "DrawSystem.h"
#include <iostream>

void DrawSystem::loadTexture(std::string name, std::string path) {
	sf::Texture* tex = new sf::Texture();
	textures.insert(std::make_pair(name, 
		tex
	));
	textures[name]->loadFromFile(path);
}

void DrawSystem::loadTextures() {
	loadTexture("backwall", "textures/backwall.png");
	loadTexture("backwall1", "textures/backwall1.png");
	loadTexture("background", "textures/background.png");
	loadTexture("ground", "textures/ground.png");
	loadTexture("ship", "textures/ship.png");
	loadTexture("mainFire", "textures/fire.png");
	loadTexture("backFire", "textures/Ffire.png");
	loadTexture("leftFire", "textures/Lfire.png");
	loadTexture("rightFire", "textures/Rfire.png");
	loadTexture("bullet", "textures/bullet.png");
	loadTexture("turret", "textures/turret.png");
	loadTexture("turretAngry", "textures/turretAngry.png");
	loadTexture("explosion", "textures/explosion.png");
	loadTexture("exit", "textures/exit.png");
	loadTexture("exit1", "textures/exit1.png");
	loadTexture("exit2", "textures/exit2.png");
	loadTexture("circle", "textures/circle.png");
	loadTexture("arrow", "textures/arrow.png");
	loadTexture("angerParticle", "textures/angerParticle.png");
	loadTexture("wall", "textures/wall.png");
	loadTexture("wallBorder", "textures/wallBorder.png");
	loadTexture("shield", "textures/shield.png");
	loadTexture("blood", "textures/blood.png");
	loadTexture("turretBody", "textures/turretBody.png");
	loadTexture("rocketLauncherBody", "textures/rocketLauncherBody.png");
	loadTexture("shieldBonus", "textures/shield.png");
	loadTexture("rocketLauncher", "textures/rocketLauncher.png");
}
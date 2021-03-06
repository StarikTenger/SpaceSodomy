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
	font.loadFromFile("font.ttf");

	loadTexture("arrow", "textures/arrow.png");
	loadTexture("pointer", "textures/pointer.png");
	loadTexture("backwall", "textures/backwall.png");
	loadTexture("backwall1", "textures/backwall1.png");
	loadTexture("background", "textures/background.png");
	loadTexture("ground", "textures/ground.png");
	loadTexture("ship", "textures/ship.png");
	loadTexture("mainFire", "textures/fire.png");
	loadTexture("backFire", "textures/Ffire.png");
	loadTexture("leftFire", "textures/Lfire.png");
	loadTexture("rightFire", "textures/Rfire.png");
	loadTexture("rightTurnFire", "textures/RTfire.png");
	loadTexture("leftTurnFire", "textures/LTfire.png");
	loadTexture("directFireRed", "textures/directFireRed.png");
	loadTexture("laser", "textures/laser.png");
	loadTexture("laserBase", "textures/laserBase.png");
	loadTexture("laserEnd", "textures/laserEnd.png");
	loadTexture("bullet", "textures/bullet.png");
	loadTexture("bulletBad", "textures/bulletBad.png");
	loadTexture("turret", "textures/turret.png");
	loadTexture("turretAngry", "textures/turretAngry.png");
	loadTexture("explosion", "textures/explosion.png");
	loadTexture("exit", "textures/exit.png");
	loadTexture("exit1", "textures/exit1.png");
	loadTexture("exit2", "textures/exit2.png");
	loadTexture("circle", "textures/circle.png");
	loadTexture("angerParticle", "textures/angerParticle.png");
	loadTexture("wall", "textures/wall.png");
	loadTexture("wallA", "textures/wallA.png");
	loadTexture("wallB", "textures/wallB.png");
	loadTexture("wallC", "textures/wallC.png");
	loadTexture("wallD", "textures/wallD.png");
	loadTexture("wallBorder", "textures/wallBorder.png");
	loadTexture("shield", "textures/shield.png");
	loadTexture("blood", "textures/blood.png");
	loadTexture("death", "textures/death.png");
	loadTexture("shipBody", "textures/shipBody.png");
	loadTexture("turretBody", "textures/turretBody.png");
	loadTexture("rocketLauncherBody", "textures/rocketLauncherBody.png");
	loadTexture("robotBody", "textures/robotBody.png");
	loadTexture("shieldBonus", "textures/shield.png");
	loadTexture("rocketLauncher", "textures/rocketLauncher.png");
	loadTexture("laserCarrier", "textures/laserCarrier.png");
	loadTexture("generatorBody", "textures/generatorBody.png");
	loadTexture("generatorLight", "textures/generatorLight.png");
	loadTexture("robot", "textures/robot.png");
	loadTexture("glowRed", "textures/glowRed.png");
	loadTexture("glowCornerRed", "textures/glowCornerRed.png");
	loadTexture("particleSmoke", "textures/particleSmoke.png");
	loadTexture("particleLaser", "textures/particleLaser.png");
	loadTexture("particleFlame", "textures/particleFlame.png");
}
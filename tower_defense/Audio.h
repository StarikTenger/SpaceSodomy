#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <map>
#include "Vector2d.h"

class Audio {
public:
	std::map<std::string, sf::Sound*> sounds;
	Audio();
	~Audio();
	void loadSound(std::string name, std::string path);
	void play(std::string name);
	void play(std::string name, double volume);
	void play(std::string name, Vector2d pos, double volume);
};


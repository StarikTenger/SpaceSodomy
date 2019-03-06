#include "Audio.h"


Audio::Audio(){
	loadSound("damage", "sounds/damage.wav");
	loadSound("launch", "sounds/launch.wav");
	loadSound("explosion", "sounds/explosion.wav");
	loadSound("fire", "sounds/fire.wav");
	loadSound("knock", "sounds/knock.wav");
	loadSound("bip", "sounds/bip.wav");
	loadSound("death", "sounds/death.wav");
}

Audio::~Audio(){
}

void Audio::loadSound(std::string name, std::string path) {
	sf::SoundBuffer* buffer = new sf::SoundBuffer();
	buffer->loadFromFile(path);
	sf::Sound* sound = new sf::Sound();
	sound->setBuffer(*buffer);
	sounds.insert(std::make_pair(name,
		sound
	));
}

void Audio::play(std::string name) {
	play(name, 100);
}
void Audio::play(std::string name, double volume) {
	sounds[name]->setVolume(volume);
	sounds[name]->play();
}
void Audio::play(std::string name, Vector2d pos, double volume) {
	sf::Listener::setDirection(1.f, 0.f, 0.f);
	sounds[name]->setPosition(5, pos.x, pos.y);
	sounds[name]->setVolume(volume);
	sounds[name]->play();
}

#include "DrawSystem.h"
#include <math.h>
#include <algorithm>

#include <iostream>

void DrawSystem::fillRect(double x, double y, double width, double height, Color color) {

}

void DrawSystem::strokeRect(double x, double y, double width, double height, Color color) {

}

void DrawSystem::image(std::string name, double x, double y, double width, double height, double angle, Color color) {
	sf::Texture& tex = *textures[name];
	Vector2d scale = { width / tex.getSize().x  , height / tex.getSize().y };
	Vector2d pos = { x, y };// cam.transform({ x, y });
	sf::Sprite sprite;
	sprite.setOrigin(tex.getSize().x / 2, tex.getSize().y / 2);
	sprite.setTexture(tex);
	sprite.setScale(scale.x, scale.y);
	sprite.setPosition(x, y);
	sprite.setColor(sf::Color(std::min(255, color.r), std::min(255, color.g), std::min(255, color.b), std::min(255, color.a)));
	sprite.setRotation(angle * 180 / M_PI);

	window->draw(sprite);
}

void DrawSystem::image(std::string name, double x, double y, double width, double height, double angle) {
	image(name, x, y, width, height, angle, Color(255, 255, 255, 255));
}

void DrawSystem::image(std::string name, double x, double y, double width, double height, double angle, double d) {
	Vector2d pos(x, y);
	pos = system->units[0]->body.pos + (pos - system->units[0]->body.pos)*d;
	image(name, pos.x, pos.y, width*d, height*d, angle);
}

void DrawSystem::animation(std::string img, AnimationState p1, AnimationState p2, double time) {
	animations.push_back(
		Animation(
			img, p1, p2, system->time, system->time + time
		)
	);
}

void DrawSystem::text(std::string text, double x, double y, int size, Color color) {
	sf::Text drawnText;
	drawnText.setFont(font);
	drawnText.setString(text);
	drawnText.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
	drawnText.setCharacterSize(size);
	drawnText.setOrigin(floor(drawnText.getLocalBounds().width / 2), floor(drawnText.getLocalBounds().height ));
	drawnText.setPosition(x, y);
	window->draw(drawnText);
}
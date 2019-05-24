#include "DrawSystem.h"
#include <math.h>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>

void DrawSystem::drawMenu() {
	//view/////////////////////////////////////////////
	w = window->getSize().x;
	h = window->getSize().y;
	double size = h / 7;
	double d = size / 2;
	window->setView(sf::View(sf::FloatRect(0, 0, w, h)));
	///////////////////////////////////////////////////
	image("background",
		w / 2,
		h / 2,
		w * 1.5,
		w * 1.5, 0,
		Color(100, 100, 100)
	);
	for (MenuButton* button : menu->buttons) {
		image("wall", button->pos.x * w, button->pos.y * h, button->box.x*h, button->box.y*h, 0);
		Color c = { 0, 151, 255 };
		if(button->selected)
			c = { 0, 255, 0 };
		text(button->text, button->pos.x * w, (button->pos.y-button->box.y/8) * h, (int)(h / 10), c);
	}
}
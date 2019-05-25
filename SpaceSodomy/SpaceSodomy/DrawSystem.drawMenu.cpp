#include "DrawSystem.h"
#include <math.h>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>

void DrawSystem::drawMenu() {
	if(system)
		drawScene();
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
		Color(0, 0, 0, 220)
	);
	for (MenuButton* button : menu->buttons) {
		if (button->type == PLAY) {
			image("wall", button->pos.x * w, button->pos.y * h, button->box.x*h, button->box.y*h, 0);
			Color c = { 0, 151, 255 };
			if (button->selected)
				c = { 0, 255, 0 };
			text(button->text, button->pos.x * w, (button->pos.y - button->box.y / 8) * h, (int)(h / 10), c);
		}
		if (button->type == LEVEL) {
			if (menu->level >= button->state) {
				image("wall", button->pos.x * w, button->pos.y * h, button->box.x*h, button->box.y*h, 0);
				Color c = { 0, 151, 255 };
				if (button->selected)
					c = { 0, 255, 0 };
				text(button->text, button->pos.x * w, (button->pos.y - button->box.y / 8) * h, (int)(h / 10), c);
			}
			else {
				image("wall", button->pos.x * w, button->pos.y * h, button->box.x*h, button->box.y*h, 0, Color( 255, 100, 100));
				Color c = {  200, 0,0 };
				if (button->selected)
					c = {  255,0, 0 };
				text(button->text, button->pos.x * w, (button->pos.y - button->box.y / 8) * h, (int)(h / 10), c);
			}
		}
	}
}
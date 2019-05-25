#include <iostream>
#include "Menu.h"

Menu::Menu() {
	buttons.push_back(new MenuButton(
		Vector2d(0.5, 0.15),
		Vector2d(0.3, 0.1),
		PLAY,
		"continue"
	));
	int width = 5;
	double size = 0.1;
	double margin = 0.02;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < width; y++) {
			int value = y * width + x;
			buttons.push_back(new MenuButton(
				Vector2d(0.5 - (size+margin)/2*width, 0.6 - (size + margin) / 2 * width) + Vector2d((size+margin*2)*x + margin, (size + margin * 2)*y + margin),
				Vector2d(size, size),
				LEVEL,
				std::to_string(value)
			));
			buttons.back()->state = value;
		}
	}
}
Menu::~Menu() {
}

void Menu::step() {
	double w = scale.x;
	double h = scale.y;
	for (MenuButton* button : buttons) {
		button->selected = 0;
		if (
			mouse->pos.x < button->pos.x * w + button->box.x * h / 2 && 
			mouse->pos.x > button->pos.x * w - button->box.x * h / 2 && 
			mouse->pos.y < button->pos.y * h + button->box.y * h / 2 &&
			mouse->pos.y > button->pos.y * h - button->box.y * h / 2 
		) {
			button->selected = 1;
			if (mouse->state && !mouse->statePrev) {
				switch (button->type) {
				case PLAY:
					events.push_back("play");
					break;
				case LEVEL:
					if(level >= button->state)
						events.push_back("level " + std::to_string(button->state));
					break;
				}
			}
			
		}
	}
}

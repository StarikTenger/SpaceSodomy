#include "Control.h"
#include <iostream>

void Control::events() {
	


	sf::Event event;
	while (drawSys.window->pollEvent(event))
	{
		switch (event.type) {
		case sf::Event::Closed:
			stop = 1;
			drawSys.window->close();
			break;
		case sf::Event::KeyPressed:
			keys[getKey(event.key.code)] = 1;
			break;
		case sf::Event::KeyReleased:
			keys[getKey(event.key.code)] = 0;
			break;
		case sf::Event::MouseMoved:
			mouse.pos.x = event.mouseMove.x;
			mouse.pos.y = event.mouseMove.y;
			break;
		case sf::Event::JoystickButtonPressed:
			joystick.get(event.joystickButton.button) = 1;
			break;
		case sf::Event::JoystickButtonReleased:
			joystick.get(event.joystickButton.button) = 0;
			break;
		case sf::Event::JoystickMoved:
			if(event.joystickMove.axis == 0)
				joystick.pos.x = event.joystickMove.position;
			else
				joystick.pos.y = event.joystickMove.position;
			break;
		}
	}
}
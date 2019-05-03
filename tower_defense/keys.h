#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum Commands {
	NONE,
	RIGHT,
	LEFT,
	DOWN,
	UP,
	SPACE,
	SHIFT,
	W, A, S, D,
	E, Q, P, R, F
};


inline Commands getKey(sf::Keyboard::Key key) {
	switch (key) {
	case sf::Keyboard::Right:
		return RIGHT;
	case sf::Keyboard::Left:
		return LEFT;
	case sf::Keyboard::Down:
		return DOWN;
	case sf::Keyboard::Up:
		return UP;
	case sf::Keyboard::Space:
		return SPACE;
	case sf::Keyboard::LShift:
		return SHIFT;

		//WASD
	case sf::Keyboard::W:
		return W;
	case sf::Keyboard::A:
		return A;
	case sf::Keyboard::S:
		return S;
	case sf::Keyboard::D:
		return D;
		//symbols
	case sf::Keyboard::R:
		return R;
	case sf::Keyboard::F:
		return F;
	case sf::Keyboard::Q:
		return Q;
	case sf::Keyboard::E:
		return E;
	}
	return NONE;
}

inline Commands getKey(std::string s) {
	if(s == "RIGHT")
		return RIGHT;
	if (s == "LEFT")
		return LEFT;
	if (s == "DOWN")
		return DOWN;
	if (s == "UP")
		return UP;
	if (s == "SPACE")
		return SPACE;
	if (s == "SHIFT")
		return SHIFT;

		//WASD
	if (s == "W")
		return W;
	if (s == "A")
		return A;
	if (s == "S")
		return S;
	if (s == "D")
		return D;
		//symbols
	if (s == "R")
		return R;
	if (s == "F")
		return F;
	if (s == "Q")
		return Q;
	if (s == "E")
		return E;
	return NONE;
}


#pragma once
#include <string>
#include "geometry.h"

enum ButtonType {
	PLAY, LEVEL
};

class MenuButton {
public:
	Vector2d pos;
	Vector2d box;
	ButtonType type = PLAY;
	std::string text;
	int state = 0;
	bool selected = 0;
	//std::string

	MenuButton();
	MenuButton(Vector2d _pos, Vector2d _box, ButtonType _type, std::string _text);
	~MenuButton();
};


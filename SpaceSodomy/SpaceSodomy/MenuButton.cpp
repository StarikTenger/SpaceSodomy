#include "MenuButton.h"

MenuButton::MenuButton() {

}

MenuButton::MenuButton(Vector2d _pos, Vector2d _box, ButtonType _type, std::string _text) {
	pos = _pos;
	box = _box;
	type = _type;
	text = _text;
}

MenuButton::~MenuButton() {

}

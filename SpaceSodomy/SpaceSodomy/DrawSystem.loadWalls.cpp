#include "DrawSystem.h"
#include <math.h>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>

void DrawSystem::loadWalls(std::string name, std::string img, System& sys) {
	if (!textures[name]) {
		int d = textures[img]->getSize().x;
		int width = textures[img]->getSize().x*sys.field.size();
		sf::Image* block = new sf::Image();
		*block = textures[img]->copyToImage();
		sf::Image* walls = new sf::Image();
		walls->create(width, width, sf::Color(0, 0, 0, 0));
		for (int x = 0; x < sys.field.size(); x++) {
			for (int y = 0; y < sys.field[0].size(); y++) {
				if (sys.field[x][y].type)
					walls->copy(*block, x*d, y*d);
			}
		}
		sf::Texture* tex = new sf::Texture();
		textures.insert(std::make_pair(name,
			tex
		));
		textures[name] = tex;
		textures[name]->loadFromImage(*walls);
		std::cout << "a " << textures[name] << "\n";
	}
}
#include <iostream>
#ifndef STARTMENU_H
#define STARTMENU_H

class StartMenu {
private:
	sf::RenderWindow* window; // Pointer to the render window
	sf::Texture startMenuTexture;
	sf::Sprite startMenuSprite;
public:
	StartMenu(sf::RenderWindow* window) :
		window(window),
		startMenuTexture(),
		startMenuSprite(startMenuTexture)
	{
		if (!startMenuTexture.loadFromFile("assets/StartMenu.png")) {
			std::cerr << "Error loading StartMenu.png" << std::endl;
		}
		startMenuSprite.setTexture(startMenuTexture);
		startMenuSprite.setPosition({ 0.f, 0.f });
	}
	void drawStartMenu() {
		window->draw(startMenuSprite);
	}
};

#endif
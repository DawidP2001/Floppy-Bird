#include <iostream>
#ifndef STARTMENU_H
#define STARTMENU_H

class StartMenu {
private:
	sf::RenderWindow* window; // Pointer to the render window
	std::vector<sf::Texture> startTextures;
public:
	StartMenu(sf::RenderWindow* window) :
		window(window),
		startTextures(206)
	{
		for (int i = 0; i < 206; ++i) {
			sf::Texture texture("assets/Start-Menu/Sequence/" + std::to_string(i) +".png");
			startTextures[i] = texture;
		}
		
	}
	void drawStartMenu() {
		sf::Sprite startMenuSprite(startTextures[0]);
		startMenuSprite.setPosition({ 0.f, 0.f });
		for (int i = 0; i < startTextures.size(); ++i) {
			startMenuSprite.setTexture(startTextures[i]);
			window->clear(sf::Color::Black);
			window->draw(startMenuSprite);
			window->display();
			sf::sleep(sf::milliseconds(60));
		}
	}
};

#endif
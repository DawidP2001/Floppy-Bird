#include <iostream>
#ifndef STARTMENU_H
#define STARTMENU_H

class StartMenu {
private:
	sf::RenderWindow* window; // Pointer to the render window
	std::vector<sf::Texture> startTextures;
	sf::Music music;
	bool* gameStarted;
public:
	StartMenu(sf::RenderWindow* window, bool* theGameStarted) :
		window(window),
		startTextures(254)
	{
		for (int i = 0; i < 254; ++i) {
			sf::Texture texture("assets/Start-Menu/Sequence/" + std::to_string(i) +".png");
			startTextures[i] = texture;
		}
		gameStarted = theGameStarted;
		music.openFromFile("assets/music/start.wav");
		music.setLooping(true);
		music.setVolume(50);   // optional (0-100)
	}
	bool checkStart() {
		while (const std::optional event = window->pollEvent()) {
			if (event->is<sf::Event::Closed>())
				window->close();
			if (event->is<sf::Event::KeyPressed>() &&
				event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Space) {
				*gameStarted = true; // Start the game
				return true;
			}
		}
		return false;
	}

	void playMusic() {
		music.play();
	}
	void drawStartMenu() {
		sf::Sprite startMenuSprite(startTextures[0]);
		startMenuSprite.setPosition({ 0.f, 0.f });
		while (!(*gameStarted)) {
			for (int i = 0; i < startTextures.size(); ++i) {
				startMenuSprite.setTexture(startTextures[i]);
				window->clear(sf::Color::Black);
				window->draw(startMenuSprite);
				window->display();
				sf::sleep(sf::milliseconds(70));
				if (checkStart()) {
					break;
				}
			}
		}
		
	}
};

#endif
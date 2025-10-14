#include <iostream>
#ifndef BACKGROUND_H
#define BACKGROUND_H

class Background {
private:
	const unsigned int screenWidth; // Example screen width
	const unsigned int screenHeight; // Example screen height
	const float scrollScreenSpeed; // Speed of background scrolling
	sf::RenderWindow* window; // Pointer to the render window

	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	sf::Texture backgroundTexture2;
	sf::Sprite backgroundSprite2;

    void createBackground() {
		backgroundSprite.setPosition({ 0.f, 0.f });
		backgroundSprite2.setPosition({ static_cast<float>(screenWidth*2), 0.f });
		backgroundSprite.setTextureRect({ sf::IntRect({0, 0}, {static_cast<int>(screenWidth * 2), static_cast<int>(screenHeight)}) });
		backgroundSprite2.setTextureRect({ sf::IntRect({0, 0}, {static_cast<int>(screenWidth * 2), static_cast<int>(screenHeight)}) });
    }
public:
	Background(unsigned int screenWidth, int screenHeight, int scrollScreenSpeed, sf::RenderWindow* window) :
		screenWidth(screenWidth),
		screenHeight(screenHeight),
		scrollScreenSpeed(static_cast<float>(scrollScreenSpeed)),
		window(window),
		backgroundTexture("assets/Background.png"),
		backgroundTexture2("assets/Background.png"),
		backgroundSprite(backgroundTexture),
		backgroundSprite2(backgroundTexture2)
	{
		createBackground();
	}
	void drawBackground() {
		window->draw(backgroundSprite);
		window->draw(backgroundSprite2);
	}
	void updateBackground() {
		backgroundSprite.move({ -scrollScreenSpeed, 0.f });
		backgroundSprite2.move({ -scrollScreenSpeed, 0.f });
		// Reset position if they go off screen
		if (backgroundSprite.getPosition().x <= -static_cast<float>(screenWidth * 2)) {
			backgroundSprite.setPosition({ static_cast<float>(screenWidth * 2), 0.f });
		}
		if (backgroundSprite2.getPosition().x <= -static_cast<float>(screenWidth * 2)) {
			backgroundSprite2.setPosition({ static_cast<float>(screenWidth * 2), 0.f });
		}
	}
};
#endif
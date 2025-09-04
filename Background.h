#include <iostream>
#ifndef BACKGROUND_H
#define BACKGROUND_H

class Background {
private:
	const unsigned int screenWidth; // Example screen width
	const unsigned int screenHeight; // Example screen height
	const int grassHeight; // Height of the grass sprite
	const float scrollScreenSpeed; // Speed of background scrolling
	sf::RenderWindow* window; // Pointer to the render window
	std::vector<sf::Sprite> bgGrassSprites; // Vector to hold grass sprites
	sf::Texture skyTexture;
	sf::Sprite skySprite1;
	sf::Sprite skySprite2;
	sf::Texture grassTexture;
	sf::Texture cloudTexture;
	sf::Sprite cloudSprite;

    void createBackground() {
        skyTexture.setRepeated(true); // Enable texture repetition for the background
        skySprite1.setPosition({ 0.f, 0.f });
        skySprite2.setPosition({ static_cast<float>(screenWidth), 0.f });
        skySprite1.setTextureRect({ sf::IntRect({0, 0}, {static_cast<int>(screenWidth), static_cast<int>(screenHeight)}) });
        skySprite2.setTextureRect({ sf::IntRect({0, 0}, {static_cast<int>(screenWidth), static_cast<int>(screenHeight)}) });

        // Create background Grass
        grassTexture.setRepeated(true); // Enable texture repetition for the grass
        for (int i = 0; i < 2; ++i) {
            sf::Sprite grassSprite(grassTexture);
            grassSprite.setPosition({ static_cast<float>(i * screenWidth), static_cast<float>(screenHeight - grassHeight) });
            grassSprite.setTextureRect({ sf::IntRect({0, 0}, {static_cast<int>(screenWidth), grassHeight}) });
            bgGrassSprites.push_back(grassSprite);
        }

		cloudSprite.setPosition({ 100.f, 50.f });
    }
public:
	Background(unsigned int screenWidth, int screenHeight, int grassHeight, int scrollScreenSpeed, sf::RenderWindow* window) :
		screenWidth(screenWidth),
		screenHeight(screenHeight),
		grassHeight(static_cast<unsigned int>(grassHeight)),
		scrollScreenSpeed(static_cast<float>(scrollScreenSpeed)),
		window(window),
		skyTexture("assets/Cielo pixelado.png"),
		skySprite1(skyTexture),
		skySprite2(skyTexture),
		grassTexture("assets/grass-set-00/grass14.png"),
		cloudTexture("assets/Clouds/Cloud-1.png"),
		cloudSprite(cloudTexture)
	{
		createBackground();
	}
	void drawBackground() {
		window->draw(skySprite1);
		window->draw(skySprite2);
		for (const auto& grassSprite : bgGrassSprites) {
			window->draw(grassSprite);
		}
		window->draw(cloudSprite);
	}
	void updateBackground() {
		skySprite1.setColor(sf::Color(255, 255, 255, 200));
		// Move the background sprites to create a scrolling effect
		skySprite1.move({ -scrollScreenSpeed, 0.f });
		skySprite2.move({ -scrollScreenSpeed, 0.f });
		// Reset position if they go off screen
		if (skySprite1.getPosition().x <= -static_cast<float>(screenWidth)) {
			skySprite1.setPosition({ static_cast<float>(screenWidth), 0.f });
		}
		if (skySprite2.getPosition().x <= -static_cast<float>(screenWidth)) {
			skySprite2.setPosition({ static_cast<float>(screenWidth), 0.f });
		}

		// Update grass sprites 
		for (auto& grassSprite : bgGrassSprites) {
			grassSprite.move({ -scrollScreenSpeed, 0.f });
			if (grassSprite.getPosition().x <= -static_cast<float>(screenWidth)) {
				grassSprite.setPosition({ static_cast<float>(screenWidth), static_cast<float>(screenHeight - grassHeight) });
			}
		}
	}
};
#endif
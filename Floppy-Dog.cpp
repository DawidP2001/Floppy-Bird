// Floppy-Dog.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Author: Dawid Pionk
// Date: 03/08/2025
// Copyright (c) 2025 Dawid Pionk. All rights reserved.

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

class Player {
private:
    sf::Texture texture;
    sf::Sprite sprite;
public:

    Player() :
        texture("assets/dog.jpg"),
		sprite(texture)
    {
        sprite.setPosition({ 100.f, 500.f });
        sprite.setScale({ 0.5f, 0.5f });
    }
    ~Player() = default;
    
    // Get the player sprite
    sf::Sprite& getSprite() {
        return sprite;
    }
    
    void jump() {
        sprite.move({ 0.f, -10.f });
    }
    
    void update() {
        sprite.move({ 0.f, 5.f });
    }
};

class Obstacle {
private:
    int width = 50;
    int height = 600;
    int gapHeight = 100;
    int gapPosition = 250;
    sf::RectangleShape topRectangle;
    sf::RectangleShape bottomRectangle;
public:
    Obstacle() :
		topRectangle({100.f, 100.f}),
        bottomRectangle({100.f, 100.f})
    {
        topRectangle.setFillColor(sf::Color::Green);
        bottomRectangle.setFillColor(sf::Color::Green);
	}
    ~Obstacle() = default;
    

    void update() {
        topRectangle.move({ -5.f, 0.f });
        bottomRectangle.move({ -5.f, 0.f });
        if(topRectangle.getPosition().x < -width) {
            // Reset position when it goes off screen
            topRectangle.setPosition({ 800.f, 0.f });
            bottomRectangle.setPosition({ 800.f, 100.f });
		}
    }
    
    // Set position for both parts
    void setPosition(float x, float y) {
        topRectangle.setPosition({ x, y });
        bottomRectangle.setPosition({ x, y + gapPosition + gapHeight });
    }
    
    // Get X position
    float getPositionX() const {
        return topRectangle.getPosition().x;
    }
    
    // Get the rectangles
    sf::RectangleShape& getTopRectangle() {
        return topRectangle;
    }
    
    sf::RectangleShape& getBottomRectangle() {
        return bottomRectangle;
    }
};

class FloppyDogGame {
private:
    sf::Font font;
    std::string scoreTextStr;
    sf::Color scoreColor;
    sf::RenderWindow window;
    sf::Text scoreText;
	Player player;
	Obstacle obstacle;

    unsigned int fontSize = 24; // Font size for the score text
    int frameCount = 60;
    unsigned int screenWidth = 800;
    unsigned int screenHeight = 600;

    void draw() {
        window.clear();

        window.draw(player.getSprite());
		window.draw(obstacle.getTopRectangle());
		window.draw(obstacle.getBottomRectangle());
        window.draw(scoreText);

        window.display();
    }
    // Check for events like key presses
    void checkEvents() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            player.jump();
        }
    }

    void update() {
        player.update();
		obstacle.update(); // Move the obstacle left
    }
public:
    FloppyDogGame() :
        font("assets/fonts/arial.ttf"), // Load the font
        scoreTextStr("Score: 0"), // Initialize score text
        scoreColor(sf::Color::White), // Set color for the score text
        scoreText(font, scoreTextStr),
        window(sf::VideoMode({ 800, 600 }), "Floppy Dog Game"),
		player(),
		obstacle()
    {
		scoreText.setCharacterSize(fontSize);
        window.setFramerateLimit(frameCount);
        obstacle.setPosition(800.f, 0.f);
    }
    
    ~FloppyDogGame() = default;
    
    void run() {
        while (window.isOpen()) {
			checkEvents();
			update();
            draw();
            // Handle window events
            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>())
                    window.close();
            }
        }
    }
};

int main() {
    FloppyDogGame game;
    game.run();
    return 0;
}
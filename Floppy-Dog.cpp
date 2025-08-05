// Floppy-Dog.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Author: Dawid Pionk
// Date: 03/08/2025
// Copyright (c) 2025 Dawid Pionk. All rights reserved.

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

// Creates a vector of obstacles
std::vector<sf::RectangleShape> createObstacles(int numObstacles) {
    std::vector<sf::RectangleShape> obstacles;
    obstacles.resize(numObstacles);
    for (int i = 0; i < numObstacles; ++i) {
        obstacles[i] = sf::RectangleShape({ 50.f, 600.f });
        obstacles[i].setPosition({ 600.f + i * 200.f, 0.f });
        obstacles[i].setFillColor(sf::Color::Green);
    }
    return obstacles;
}

std::vector<sf::RectangleShape> static createGaps(int numGaps) {
    std::vector<sf::RectangleShape> gaps;
    gaps.resize(numGaps);
    for (int i = 0; i < numGaps; ++i) {
        gaps[i] = sf::RectangleShape({ 50.f, 100.f });
        gaps[i].setFillColor(sf::Color::White);
        gaps[i].setPosition({ 600.f + i * 200.f, 200.f });
    }
    return gaps;
}

sf::Text static createText(const std::string& text, const sf::Font& font, unsigned int size, const sf::Color& color) {
    sf::Text sfText(font, text);
    sfText.setCharacterSize(size);
    sfText.setFillColor(color);
	sfText.setPosition({ 10.f, 10.f }); // Set position of the text
    return sfText;
}

void static updateObstacles(std::vector<sf::RectangleShape>& obstacles, std::vector<sf::RectangleShape>& gaps) {
    for (int i = 0; i < obstacles.size(); ++i) {
        obstacles[i].move({-5.f, 0.f});
        gaps[i].move({-5.f, 0.f});
        if (obstacles[i].getPosition().x < -50.f) {
            obstacles[i].setPosition({ 600.f + obstacles.size() * 200.f, 0.f});
            gaps[i].setPosition({ 600.f + obstacles.size() * 200.f, 200.f});
        }
    }
}

void static updatePlayer(sf::Sprite& sprite) {
    sprite.move({ 0.f, 5.f });
}

void static update(sf::Sprite& sprite, std::vector<sf::RectangleShape>& obstacles, std::vector<sf::RectangleShape>& gaps) {
	updatePlayer(sprite);
	updateObstacles(obstacles, gaps);
}

void static jump(sf::Sprite& sprite) {
	sprite.move({ 0.f, -10.f }); // Move the sprite up by 50 pixels
}

// Checks for events like key presses
void static checkEvents(sf::Sprite& sprite) {
    // Check for jump
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
    {
		jump(sprite);
    }
}

void static draw(
    sf::RenderWindow& window, 
    sf::Sprite& sprite, 
    std::vector<sf::RectangleShape>& obstacles, 
    std::vector<sf::RectangleShape>& gaps,
	sf::Text& scoreText
) {
    window.clear();
    for (size_t i = 0; i < obstacles.size(); ++i) {
        window.draw(obstacles[i]);
        window.draw(gaps[i]);
    }
    window.draw(sprite);
    window.draw(scoreText);
    window.display();
}

void static checkCollision() {
	// Logic for checking collision between the sprite and obstacles
}

int main()
{
    int score = 0; 
	int numObstacles = 5; // Number of obstacles
	const sf::Font font("assets/fonts/arial.ttf"); // Load a font for text rendering
	std::string scoreTextStr = "Score: " + std::to_string(score); // Initial score text
	int fontSize = 30; // Font size for the score text
	sf::Color scoreColor = sf::Color::White; // Color for the score text
	sf::Text scoreText = createText(scoreTextStr, font, fontSize, scoreColor);

    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "My window");
    window.setFramerateLimit(60);
    sf::Texture texture("assets/dog.jpg");
    sf::Sprite sprite(texture);
	std::vector<sf::RectangleShape> obstacles = createObstacles(numObstacles);
	std::vector<sf::RectangleShape> gaps = createGaps(numObstacles);

    while (window.isOpen())
	{
		checkEvents(sprite);
		update(sprite, obstacles, gaps);
		draw(window, sprite, obstacles, gaps, scoreText);

        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }
    }
}
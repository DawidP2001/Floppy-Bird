// Floppy-Dog.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Author: Dawid Pionk
// Date: 03/08/2025
// Copyright (c) 2025 Dawid Pionk. All rights reserved.

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

sf::RectangleShape static createObstacle() {
    sf::RectangleShape obstacle({ 50.f, 600.f });
    obstacle.setFillColor(sf::Color::Green);
    obstacle.setPosition({ 600.f, 0.f });
    return obstacle;
}   
sf::RectangleShape static createGap() {
    sf::RectangleShape obstacleGap({ 50.f, 100.f });
    obstacleGap.setFillColor(sf::Color::White);
    obstacleGap.setPosition({ 600.f, 200.f });
	return obstacleGap;

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

void static drawObstacles(sf::RenderWindow& window) {

}

void static draw(sf::RenderWindow& window) {
    drawObstacles(window);
}

void static checkCollision() {

}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "My window");
    window.setFramerateLimit(60);
    sf::Texture texture("assets/dog.jpg");
    sf::Sprite sprite(texture);
	sf::RectangleShape obstacle = createObstacle();
	sf::RectangleShape obstacleGap = createGap();

    while (window.isOpen())
	{
		window.clear(); 
		checkEvents(sprite);
		sprite.move({ 0.f, 5.f }); 
        window.draw(sprite);
		window.draw(obstacle);
		window.draw(obstacleGap);
		window.display();

        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }
    }
}
// Floppy-Dog.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Author: Dawid Pionk
// Date: 03/08/2025
// Copyright (c) 2025 Dawid Pionk. All rights reserved.

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

void jump(sf::Sprite& sprite) {
	sprite.move({ 0.f, -50.f }); // Move the sprite up by 50 pixels
}

// Checks for events like key presses
void checkEvents(sf::Sprite& sprite) {
    // Check for jump
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
    {
		jump(sprite);
    }
}

void draw(sf::RenderWindow window) {
}

void checkCollision() {

}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "My window");
    window.setFramerateLimit(60);
    sf::Texture texture("assets/dog.jpg");
    sf::Sprite sprite(texture);

    // run the program as long as the window is open
    while (window.isOpen())
	{
		window.clear(); // clear the window with default color (black)
		checkEvents(sprite);
		sprite.move({ 10.f, 10.f }); // Move the sprite down by 1 pixel each frame
        window.draw(sprite);
		window.display(); // display what has been drawn to the windowor
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }
    }
}
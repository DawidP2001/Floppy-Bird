// Floppy-Dog.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Author: Dawid Pionk
// Date: 03/08/2025
// Copyright (c) 2025 Dawid Pionk. All rights reserved.

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Background.h"
#include "Player.h"
#include "Obstacle.h"
#include <vector>

class FloppyDogGame {
private:
    const int numOfObstacles = 2; // Number of obstacles to create
    const unsigned int fontSize = 24; // Font size for the score text
    const int frameCount = 60;
    const unsigned int screenWidth = 800;
    const unsigned int screenHeight = 600;
    const float scrollScreenSpeed = 5; // Speed of the scrolling screen
    const int bgGrassSpriteHeight = 200;
    const float obstacleSpacing = 450; // The screen size is 800 plus obstacle 100 so the spacing number needs to keep that in mind
    int score = 0; // Player score
    bool gameOver = false; // Game over state
    bool gameStarted = false; // Game started state

    sf::Font font;
    std::string scoreTextStr;
    sf::Color scoreColor;
    sf::RenderWindow window;
    sf::Text scoreText;
	Player player;
    std::vector<Obstacle> obstacles;
    Background background; // Background object

    ///////////////////////
    // DRAW FUNCTIONS
    ///////////////////////
    void drawObstacles() {
        for (Obstacle& obstacle : obstacles) {
            window.draw(obstacle.getTopRectangle());
            window.draw(obstacle.getBottomRectangle());
            window.draw(obstacle.getSprite());
			window.draw(obstacle.getGapRectangle());
        }
    }
    void draw() {
        window.clear();

        background.drawBackground();
        drawObstacles();
        window.draw(scoreText);
        window.draw(player.getSprite());

        window.display();
    }
    ///////////////////////
    // CREATE FUNCTIONS
    ///////////////////////
    void createObstacles(const int numberOfObstacles) {
        // Create a new obstacle and add it to the vector
        for (int i = 0; i < numberOfObstacles; ++i) {
			Obstacle newObstacle = *new Obstacle(); // I keep the object on the stack for texture reuse and simpler management
            newObstacle.setPosition(800.f + i * obstacleSpacing, 0.f); // Position them spaced out
            obstacles.push_back(newObstacle);
		}
	}

    ///////////////////////
	// UPDATE FUNCTIONS
    ///////////////////////
    void updateObstacles() {
        for (Obstacle& obstacle : obstacles) {
            obstacle.update();
        }
    }
    void update() {
        background.updateBackground();
        player.update();
		updateObstacles();
    }

    ///////////////////////
    // UTILITY FUNCTIONS
    ///////////////////////
    void reset() {
		score = 0; // Reset score
        scoreTextStr = "Score: 0"; // Reset score text
        scoreText.setString(scoreTextStr);
        obstacles.clear(); // Clear existing obstacles
        createObstacles(numOfObstacles); // Create new obstacles
		player.getSprite().setPosition({ 100.f, 500.f }); // Reset player position
    }
    void collisionDetection() {
        for (Obstacle& obstacle : obstacles) {
            sf::FloatRect playerBounds = player.getSprite().getGlobalBounds();
            sf::FloatRect topBounds = obstacle.getTopRectangle().getGlobalBounds();
            sf::FloatRect bottomBounds = obstacle.getBottomRectangle().getGlobalBounds();

            if (playerBounds.findIntersection(topBounds) || playerBounds.findIntersection(bottomBounds)) {
                //reset(); // Reset the game if collision occurs
            }
            else if (playerBounds.position.x > topBounds.position.x && !obstacle.hasScored()) {
                // if the player doesn't die and is pass the x axis of the top obstacle +1 point
                ++score;
                obstacle.setScored(); // Mark this obstacle as scored
                scoreTextStr = "Score: " + std::to_string(score);
                scoreText.setString(scoreTextStr);
            }

        }
    }
    ///////////////////////
    // DEBUGGING FUNCTIONS
    ///////////////////////
    void showDistance() {
		float distance = obstacles[0].getTopRectangle().getPosition().x - obstacles[1].getSprite().getPosition().x;
		const std::string distanceStr = "Distance: " + std::to_string(distance);
		sf::Text distanceText(font, distanceStr);
        distanceText.setCharacterSize(fontSize);
        distanceText.setFillColor(sf::Color::White);
        distanceText.setPosition({ 500.f, 10.f });
		window.draw(distanceText); // Draw the distance text
    }
    ///////////////////////
    // SCREEN FUNCTIONS
    ///////////////////////
    void showStartScreen() {
		const std::string TitleStr = "Floppy Dog Game";
		const std::string startTextStr = "Press Space to Start";
		const float titleTextY = 100.f; // Y position for the title text
		const float startTextY = 300.f; // Y position for the start text

		sf::Text titleText(font, TitleStr);
		titleText.setCharacterSize(48);
        titleText.setFillColor(sf::Color::White);
        titleText.setStyle(sf::Text::Bold);
		titleText.setPosition({ static_cast<float>(screenWidth) / 2 - titleText.getGlobalBounds().size.x / 2, titleTextY });
        sf::Text startText(font, startTextStr);
		startText.setCharacterSize(24);
        startText.setFillColor(sf::Color::White);
		startText.setPosition({ static_cast<float>(screenWidth) / 2 - startText.getGlobalBounds().size.x / 2, startTextY });

		window.clear(sf::Color::Black);
		window.draw(titleText); // Draw the title text
		window.draw(startText); // Draw the score text
        window.display();
    }
    void showDeathScreen() {
        const std::string TitleStr = "GAME OVER";
        const std::string startTextStr = "Press R to Restart";
        const float titleTextY = 100.f; // Y position for the title text
        const float startTextY = 300.f; // Y position for the start text

        sf::Text titleText(font, TitleStr);
        titleText.setCharacterSize(48);
        titleText.setFillColor(sf::Color::White);
        titleText.setStyle(sf::Text::Bold);
        titleText.setPosition({ static_cast<float>(screenWidth) / 2 - titleText.getGlobalBounds().size.x / 2, titleTextY });
        sf::Text startText(font, startTextStr);
        startText.setCharacterSize(24);
        startText.setFillColor(sf::Color::White);
        startText.setPosition({ static_cast<float>(screenWidth) / 2 - startText.getGlobalBounds().size.x / 2, startTextY });

        window.clear(sf::Color::Black);
        window.draw(titleText); // Draw the title text
        window.draw(startText); // Draw the score text
        window.display();
    }
public:
    FloppyDogGame() :
        font("assets/fonts/arial.ttf"), // Load the font
        scoreTextStr("Score: 0"), // Initialize score text
        scoreColor(sf::Color::White), // Set color for the score text
        scoreText(font, scoreTextStr),
        window(sf::VideoMode({ 800, 600 }), "Floppy Dog Game"),
		player(),
		background(screenWidth, screenHeight, scrollScreenSpeed, &window)
    {
		scoreText.setCharacterSize(fontSize);
        window.setKeyRepeatEnabled(false);
        window.setFramerateLimit(frameCount);
		createObstacles(numOfObstacles); // Create initial obstacles
    }
    
    ~FloppyDogGame() = default;
    
    void run() {
        while (window.isOpen()) {
            if(gameStarted){
            collisionDetection();
			update();
            draw();
            } 
            else {
                showStartScreen();
			}
            // Handle window events
            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>())
                    window.close();
                if (event->is<sf::Event::KeyPressed>() && 
                    event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Space) {
                    if (gameStarted) {
                        player.jump();
                    }
                    else {
                        gameStarted = true; // Start the game
                    }
				}
            }
        }
    }
};

int main() {
    FloppyDogGame game;
    game.run();
    return 0;
}
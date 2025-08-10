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
    const float width = 100;
    const int height = 600;
    const int gapHeight = 100;
    int gapPosition = 250;
	bool scored = false;

    sf::RectangleShape topRectangle;
    sf::RectangleShape bottomRectangle;
public:
    Obstacle() :
		topRectangle({ width, static_cast<float>(gapPosition) }),
        bottomRectangle({ width,  static_cast<float>(height)})
    {
        topRectangle.setFillColor(sf::Color::Green);
        bottomRectangle.setFillColor(sf::Color::Green);
	}
    ~Obstacle() = default;
    

    void update() {
        topRectangle.move({ -5.f, 0.f });
        bottomRectangle.move({ -5.f, 0.f });
        if(topRectangle.getPosition().x < -width) {
			gapPosition = rand() % (height - gapHeight); // Generate new gap position
            topRectangle.setPosition({ 800.f, 0.f });
			topRectangle.setSize({ 100.f, static_cast<float>(gapPosition) });
            bottomRectangle.setPosition({ 800.f, static_cast<float>(gapPosition+gapHeight) });
			resetScored();
		}
    }
    
    // Set position for both parts
    void setPosition(float x, float y) {
        topRectangle.setPosition({ x, y });
        bottomRectangle.setPosition({ x, y + gapPosition + gapHeight });
    }
    
    void setScored() {
        scored = true;
	}
    void resetScored() {
        scored = false;
    }
    bool hasScored() const {
        return scored;
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
    std::vector<Obstacle> obstacles;
    // Speed
    // Gravity

	int score = 0; // Player score
	const int numOfObstacles = 3; // Number of obstacles to create
    const unsigned int fontSize = 24; // Font size for the score text
    const int frameCount = 60;
    const unsigned int screenWidth = 800;
    const unsigned int screenHeight = 600;

    void drawObstacles() {
        for (Obstacle& obstacle : obstacles) {
            window.draw(obstacle.getTopRectangle());
            window.draw(obstacle.getBottomRectangle());
        }
    }
    void updateObstacles() {
        for (Obstacle& obstacle : obstacles) {
            obstacle.update();
        }
	}   
    void createObstacles(const int numberOfObstacles) {
        // Create a new obstacle and add it to the vector
        for (int i = 0; i < numberOfObstacles; ++i) {
            Obstacle newObstacle;
            newObstacle.setPosition(800.f + i * 300.f, 0.f); // Position them spaced out
            obstacles.push_back(newObstacle);
		}
	}
    void draw() {
        window.clear();

        window.draw(player.getSprite());
		drawObstacles();
        window.draw(scoreText);

        window.display();
    }
    // Replace the collisionDetection() method with the following:

    void collisionDetection() {
        for (Obstacle& obstacle : obstacles) {
            sf::FloatRect playerBounds = player.getSprite().getGlobalBounds();
            sf::FloatRect topBounds = obstacle.getTopRectangle().getGlobalBounds();
            sf::FloatRect bottomBounds = obstacle.getBottomRectangle().getGlobalBounds();
			
            if (playerBounds.findIntersection(topBounds) || playerBounds.findIntersection(bottomBounds)) {
                std::cout << "Collision detected!" << std::endl;
            }
            else if (playerBounds.position.x > topBounds.position.x && !obstacle.hasScored()) {
                ++score;
				obstacle.setScored(); // Mark this obstacle as scored
                scoreTextStr = "Score: " + std::to_string(score);
                scoreText.setString(scoreTextStr);
            }
            
        }
    }
    // Check for events like key presses
    void checkEvents() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            player.jump();
        }
    }

    void update() {
        player.update();
		updateObstacles();
    }
public:
    FloppyDogGame() :
        font("assets/fonts/arial.ttf"), // Load the font
        scoreTextStr("Score: 0"), // Initialize score text
        scoreColor(sf::Color::White), // Set color for the score text
        scoreText(font, scoreTextStr),
        window(sf::VideoMode({ 800, 600 }), "Floppy Dog Game"),
		player()
    {
		scoreText.setCharacterSize(fontSize);
        window.setFramerateLimit(frameCount);
		createObstacles(numOfObstacles); // Create initial obstacles
    }
    
    ~FloppyDogGame() = default;
    
    void run() {
        while (window.isOpen()) {
			checkEvents();
            collisionDetection();
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
// Floppy-Dog.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Author: Dawid Pionk
// Date: 03/08/2025
// Copyright (c) 2025 Dawid Pionk. All rights reserved.

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

class Player {
public:
    sf::Texture texture;
    sf::Sprite sprite;
    int width = 50;
    int height = 50;
    
    Player() = default;
    ~Player() = default;
    
    // Initialize the player
    void init() {
        if (!texture.loadFromFile("assets/dog.jpg")) {
            std::cerr << "Error loading texture!" << std::endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition({ 100.f, 500.f });
        sprite.setScale({ 0.5f, 0.5f });
    }
    
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
public:
    int width = 50;
    int height = 600;
    int gapHeight = 100;
    int gapPosition = 250;
    sf::RectangleShape topRectangle;
    sf::RectangleShape bottomRectangle;
    
    Obstacle() = default;
    ~Obstacle() = default;
    
    // Initialize the obstacle with a gap
    void init(float x = 600.f) {
        // Top part of the obstacle
        topRectangle.setSize({ static_cast<float>(width), static_cast<float>(gapPosition) });
        topRectangle.setPosition({ x, 0.f });
        topRectangle.setFillColor(sf::Color::Green);
        
        // Bottom part of the obstacle
        float bottomHeight = height - gapPosition - gapHeight;
        bottomRectangle.setSize({ static_cast<float>(width), bottomHeight });
        bottomRectangle.setPosition({ x, static_cast<float>(gapPosition + gapHeight) });
        bottomRectangle.setFillColor(sf::Color::Green);
    }
    
    // Move both parts of the obstacle
    void move(float x, float y) {
        topRectangle.move({ x, y });
        bottomRectangle.move({ x, y });
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

// Create a text object
sf::Text createText(const std::string& text, const sf::Font& font, unsigned int size, const sf::Color& color) {
    sf::Text sfText(font, text);
    sfText.setCharacterSize(size);
    sfText.setFillColor(color);
    sfText.setPosition({ 10.f, 10.f });
    return sfText;
}

// Creates a vector of obstacles
std::vector<Obstacle> createObstacles(int numObstacles) {
    std::vector<Obstacle> obstacles;
    obstacles.resize(numObstacles);
    for (int i = 0; i < numObstacles; ++i) {
        obstacles[i].init(600.f + i * 200.f);
    }
    return obstacles;
}

void updateObstacles(std::vector<Obstacle>& obstacles) {
    for (auto& obstacle : obstacles) {
        obstacle.move(-5.f, 0.f);
        if (obstacle.getPositionX() < -50.f) {
            obstacle.setPosition(850.f, 0.f);
        }
    }
}

class FloppyDogGame {
private:
    sf::Font font;
    sf::Text scoreText;
    std::vector<Obstacle> obstacles;
    Player player;
    
public:
    const float windowWidth = 800.f;
    const float windowHeight = 600.f;
    int score;
    sf::RenderWindow window;

    // Remove = default and provide custom constructor
    FloppyDogGame() : window(sf::VideoMode({ 800, 600 }), "Floppy Dog"), score(0) {
        window.setFramerateLimit(60);
    }
    
    ~FloppyDogGame() = default;

    // Initialize the game
    void init() {
        // Remove window initialization since it's done in constructor
        
        // Load font
        if (!font.openFromFile("assets/fonts/arial.ttf")) {
            std::cerr << "Error loading font!" << std::endl;
        }
        
        // Initialize player
        player.init();
        
        // Create score text
        std::string scoreTextStr = "Score: " + std::to_string(score);
        scoreText = createText(scoreTextStr, font, 30, sf::Color::White);
        
        // Create obstacles
        int numObstacles = 5;
        obstacles = createObstacles(numObstacles);
    }

    // Check for events like key presses
    void checkEvents() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            player.jump();
        }
    }

    void update() {
        player.update();
        updateObstacles(obstacles);
    }

    // Draw the game elements
    void draw() {
        window.clear();
        
        // Draw obstacles
        for (auto& obstacle : obstacles) {
            window.draw(obstacle.getTopRectangle());
            window.draw(obstacle.getBottomRectangle());
        }
        
        // Draw player
        window.draw(player.getSprite());
        
        // Draw score
        window.draw(scoreText);
        
        window.display();
    }

    // Run the game loop
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
    game.init();
    game.run();
    return 0;
}
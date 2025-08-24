// Floppy-Dog.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Author: Dawid Pionk
// Date: 03/08/2025
// Copyright (c) 2025 Dawid Pionk. All rights reserved.

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

class Player {
private:
    sf::Texture stand_Texture;
	sf::Texture jump_Texture;
    sf::Sprite sprite;
	float veolocityY = 0.f; // Vertical velocity for jumping
	float gravity = 0.5f; // Gravity effect
	float jumpHeight = -8.f; // Height of the jump
public:

    Player() :
        stand_Texture("assets/milo-stand.png"),
		jump_Texture("assets/milo-jump.png"),
		sprite(stand_Texture)
    {
        sprite.setPosition({ 100.f, 500.f });
        sprite.setScale({ 1.f, 1.f });
    }
    ~Player() = default;
    
    // Get the player sprite
    sf::Sprite& getSprite() {
        return sprite;
    }
    
    void jump() {
        veolocityY = jumpHeight; // Set the vertical velocity to jump height
    }
	// Update the player image based on the vertical velocity
    void updateImage() {
		sprite.setTexture(veolocityY < 0 ? jump_Texture : stand_Texture);
    }

    void update() {
        updateImage();
        if (sprite.getPosition().y < 0) {
            sprite.setPosition({ sprite.getPosition().x, 0.f }); // Prevent going above the top
            veolocityY = 0.f; // Reset vertical velocity
        }
        else if( sprite.getPosition().y + sprite.getGlobalBounds().size.y > 600) {
            if (veolocityY > 0.f) {
                sprite.setPosition({ sprite.getPosition().x, 600.f - sprite.getGlobalBounds().size.y });
                veolocityY = 0.f;
            }
		}
		veolocityY += gravity; // Apply gravity to the vertical velocity
        sprite.move({ 0.f, veolocityY });
    }
};

class Obstacle {
private:
    const float width = 100;
    const int height = 600;
    const int gapHeight = 150;
    int gapPosition = 250;
	bool scored = false; // This variable is used to check if the player has scored by passing the obstacle, so it wouldn't be counted multiple times

    sf::Texture texture;
    sf::Sprite textureSprite;
	sf::RectangleShape gapRectangle;
    sf::RectangleShape topRectangle;
    sf::RectangleShape bottomRectangle;
public:
    Obstacle() :
		gapRectangle({ static_cast<float>(width), static_cast<float>(gapHeight) }),
		topRectangle({ width, static_cast<float>(gapPosition) }),
        bottomRectangle({ width,  static_cast<float>(height)}),
        texture("assets/BrickWall.png"), 
        textureSprite(texture)
    {
        texture.setRepeated(true);
        textureSprite.setTextureRect({ {0, 0}, {static_cast<int>(width), height} });
		gapRectangle.setFillColor(sf::Color(0, 0, 0, 128));
	}
    ~Obstacle() = default;
    

    /// Positioning after RESET NEEDS TO BE FIXED
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
		textureSprite.setPosition(topRectangle.getPosition());
		gapRectangle.setPosition({ topRectangle.getPosition().x, topRectangle.getPosition().y + gapPosition });
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
    sf::Sprite& getSprite() {
        return textureSprite;
    }
    sf::RectangleShape& getGapRectangle() {
        return gapRectangle;
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
	sf::Texture bgWallTexture;
    sf::Sprite bgWallSprite1;
	sf::Sprite bgWallSprite2; // 2 sprites for a scrolling background
	sf::Texture bgGrassTexture;
    std::vector<sf::Sprite> bgGrassSprites;

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

    ///////////////////////
    // DRAW FUNCTIONS
    ///////////////////////
    void drawBackground() {
		window.draw(bgWallSprite1);
		window.draw(bgWallSprite2);
        for (const auto& grassSprite : bgGrassSprites) {
            window.draw(grassSprite);
        }
	}
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

        drawBackground();
        drawObstacles();
        window.draw(scoreText);
        window.draw(player.getSprite());

        window.display();
    }
    ///////////////////////
    // CREATE FUNCTIONS
    ///////////////////////
    void createBackground() {
        // Create background Wall
        bgWallTexture.setRepeated(true); // Enable texture repetition for the background
        bgWallSprite1.setPosition({ 0.f, 0.f });
        bgWallSprite2.setPosition({ static_cast<float>(screenWidth), 0.f });
        bgWallSprite1.setTextureRect({ sf::IntRect({0, 0}, {static_cast<int>(screenWidth), static_cast<int>(screenHeight)}) });
        bgWallSprite2.setTextureRect({ sf::IntRect({0, 0}, {static_cast<int>(screenWidth), static_cast<int>(screenHeight)}) });

		// Create background Grass
        bgGrassTexture.setRepeated(true); // Enable texture repetition for the grass
        for (int i = 0; i < 2; ++i) {
            sf::Sprite grassSprite(bgGrassTexture);
            grassSprite.setPosition({ static_cast<float>(i * screenWidth), static_cast<float>(screenHeight - bgGrassSpriteHeight) });
            grassSprite.setTextureRect({ sf::IntRect({0, 0}, {static_cast<int>(screenWidth), bgGrassSpriteHeight}) });
            bgGrassSprites.push_back(grassSprite);
        }
	}   
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
    void updateBackground() {
        // Move the background sprites to create a scrolling effect
        bgWallSprite1.move({ -scrollScreenSpeed, 0.f });
        bgWallSprite2.move({ -scrollScreenSpeed, 0.f });
        // Reset position if they go off screen
        if (bgWallSprite1.getPosition().x <= -static_cast<float>(screenWidth)) {
            bgWallSprite1.setPosition({ static_cast<float>(screenWidth), 0.f });
        }
        if (bgWallSprite2.getPosition().x <= -static_cast<float>(screenWidth)) {
            bgWallSprite2.setPosition({ static_cast<float>(screenWidth), 0.f });
        }

		// Update grass sprites 
        for (auto& grassSprite : bgGrassSprites) {
            grassSprite.move({ -scrollScreenSpeed, 0.f });
            if (grassSprite.getPosition().x <= -static_cast<float>(screenWidth)) {
                grassSprite.setPosition({ static_cast<float>(screenWidth), static_cast<float>(screenHeight - bgGrassSpriteHeight) });
            }
		}
	}
    void update() {
        updateBackground();
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
        bgWallTexture("assets/Cielo pixelado.png"),
		bgWallSprite1(bgWallTexture),
        bgWallSprite2(bgWallTexture),
		bgGrassTexture("assets/grass-set-00/grass14.png")
    {
		scoreText.setCharacterSize(fontSize);
        window.setKeyRepeatEnabled(false);
        window.setFramerateLimit(frameCount);
        createBackground();
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
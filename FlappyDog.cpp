/* Floppy - Dog.cpp : This file contains the 'main' function.Program execution begins and ends there.
    Author: Dawid Pionk
    Date: 03/08/2025
    Copyright (c) 2025 Dawid Pionk. All rights reserved.

    To Do:
       - Make menu look better
	   - Add sound effects
       - Implement death
	   - Implement high score tracking using highScore.txt
       - Implement skins
		- Make sound effects play at the right time

    Reminder:
        Last thing i DId was work on the jump.wav file
*/
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Background.h"
#include "Player.h"
#include "Obstacle.h"
#include <vector>
#include "StartMenu.h"
#include <fstream>

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
    int highScore;
    bool gameOver = false; // Game over state
    bool gameStarted = false; // Game started state
	bool gameOverScreen = false; // So the game over screen only shows once
	bool musicPlaying = false; // Music playing 
	bool deathSoundPlayed = false; // Death sound played

    sf::Font font;
    std::string scoreTextStr;
    sf::Color scoreColor;
    sf::RenderWindow window;
    sf::Text scoreText;
	Player player;
    std::vector<Obstacle> obstacles;
    Background background; // Background object
    StartMenu startMenu;
	sf::Music gameMusic;
	sf::SoundBuffer scoreBuffer;
	sf::Sound scoreSound;
	sf::SoundBuffer deathBuffer;
	sf::Sound deathSound;

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
		drawScore();
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
    void setHighScore(const std::string& filename, int highscore) {
        std::ofstream file(filename);

        if (file.is_open()) {
            file << highscore;
            file.close();
        }
        else {
            std::cout << "Could not save high score!\n";
        }
    }
    int loadHighScore(const std::string& filename) {
        std::ifstream file(filename);

        int highscore = 0;

        if (file.is_open()) {
            file >> highscore;   // read the integer
            file.close();
        }
        else {
            std::cout << "Could not open file. Starting with default score of 0.\n";
        }

        return highscore;
    }
    /*Detects whether milo colided with the brick wall*/
    void collisionDetection() {
        for (Obstacle& obstacle : obstacles) {
            sf::FloatRect playerBounds = player.getSprite().getGlobalBounds();
            sf::FloatRect topBounds = obstacle.getTopRectangle().getGlobalBounds();
            sf::FloatRect bottomBounds = obstacle.getBottomRectangle().getGlobalBounds();

            if (playerBounds.findIntersection(topBounds) || playerBounds.findIntersection(bottomBounds)) {
				gameOver = true;
				gameOverScreen = true; // So the game over screen shows again
            }
            else if (playerBounds.position.x > topBounds.position.x && !obstacle.hasScored()) {
                // if the player doesn't die and is pass the x axis of the top obstacle +1 point
                ++score;
				scoreSound.play(); // Play score sound
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
    void showDeathScreen() {
		highScore = loadHighScore("assets/highScore.txt");
		gameOverScreen = false;
		const sf::Texture deathScreenTexture = *new sf::Texture("assets/GameOver.png");
		sf::Color textColor = sf::Color::Red;
        textColor.r = 136;
		textColor.g = 6;
        textColor.b = 6;
		sf::Sprite deathScreenSprite(deathScreenTexture);
        deathScreenSprite.setPosition({ 0.f, 0.f });

        const sf::Vector2f scoreTextLocation = { 340.f, 260.f };
        const std::string scoreStr = "Score: " + std::to_string(score);
        sf::Text scoreText(font, scoreStr);
        scoreText.setCharacterSize(28);
        scoreText.setFillColor(textColor);
        scoreText.setStyle(sf::Text::Bold);
        scoreText.setPosition(scoreTextLocation);

        const sf::Vector2f highScoreTextLocation = { 310.f, 310.f };
        const std::string highScoreStr = "High Score: " + std::to_string(highScore);
        sf::Text highScoreText(font, highScoreStr);
        highScoreText.setCharacterSize(28);
        highScoreText.setFillColor(textColor);
        highScoreText.setStyle(sf::Text::Bold);
        highScoreText.setPosition(highScoreTextLocation);
		if (score > highScore) {
			setHighScore("assets/highScore.txt", score);
		}
        window.draw(deathScreenSprite); // Draw the title text
        window.draw(scoreText);
        window.draw(highScoreText);
        window.display();
    }
    void drawScore() {
        sf::Font font("assets/fonts/arial.ttf");
        std::string textStr("HighScore: " + std::to_string(highScore));
		sf::Color textColor = sf::Color::White;
		sf::Text text(font, textStr);
		text.setCharacterSize(fontSize);
		text.setFillColor(textColor);
		text.setPosition({ 620.f, 0.f });
		window.draw(text); // Draw the score text
    }
    ///////////////////////
    // Animation FUNCTIONS
    ///////////////////////
	void deathAnimation() {
		// Simple death animation: make the player fall down
        sf::sleep(sf::milliseconds(30));
		player.changeImageToFall();
		float dropSpeed = 5.f;
		const float acceleration = 1.05f;
		while (player.getSprite().getPosition().y < static_cast<float>(screenHeight)) {
            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>())
                    window.close();
            }
			player.getSprite().move({ 0.f, dropSpeed }); // Move down by 5 pixels
			dropSpeed = dropSpeed * acceleration; // Accelerate the fall
			draw(); // Redraw the screen
			sf::sleep(sf::milliseconds(30)); // Pause for a short duration
		}
	}
public:
    FloppyDogGame() :
        font("assets/fonts/arial.ttf"), // Load the font
        scoreTextStr("Score: 0"), // Initialize score text
        scoreColor(sf::Color::White), // Set color for the score text
        scoreText(font, scoreTextStr),
        window(sf::VideoMode({ 800, 600 }), "Flappy Dog"),
		player(),
		background(screenWidth, screenHeight, scrollScreenSpeed, &window),
		startMenu(&window, &gameStarted),
		scoreBuffer("assets/Sound/point.wav"),
		scoreSound(scoreBuffer),
		deathBuffer("assets/Sound/death.wav"),
		deathSound(deathBuffer)
    {
		scoreText.setCharacterSize(fontSize);
        window.setKeyRepeatEnabled(false);
        window.setFramerateLimit(frameCount);
		createObstacles(numOfObstacles); // Create initial obstacles
		gameMusic.openFromFile("assets/music/play.wav");
        gameMusic.setLooping(true);
        gameMusic.setVolume(50);   // optional (0-100)
        deathSound.setVolume(50);
		highScore = loadHighScore("assets/highScore.txt");
    }
    
    ~FloppyDogGame() = default;
    
    void run() {
        while (window.isOpen()) {
            if (gameOver) {
                if (!deathSoundPlayed) {
                    deathSound.play(); // Play death sound
                    musicPlaying = false;
                    gameMusic.stop();
					deathSoundPlayed = true;
                }
                deathAnimation();
                if (gameOverScreen){
                    showDeathScreen(); 
                }
			}
            else if(gameStarted){
				if (!musicPlaying) {
					gameMusic.play();
					musicPlaying = true;
				}
                collisionDetection();
			    update();
                draw();
            } 
            else {
				startMenu.playMusic();
                startMenu.drawStartMenu();
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
                if (event->is<sf::Event::KeyPressed>() &&
                    event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::R) {
                    if (gameOver) {
                        gameOver = false;
						deathSoundPlayed = false;
						gameOverScreen = false;
                        reset();
                        gameStarted = true;
                        break;
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
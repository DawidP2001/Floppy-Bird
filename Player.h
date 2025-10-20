#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Audio.hpp>

class Player {
private:
    sf::Texture stand_Texture;
    sf::Texture jump_Texture;
    sf::Sprite sprite;
    sf::SoundBuffer jumpBuffer;
    sf::Sound* jumpSound;
    float veolocityY = 0.f; // Vertical velocity for jumping
    float gravity = 0.5f; // Gravity effect
    float jumpHeight = -8.f; // Height of the jump
public:

    Player() :
        stand_Texture("assets/milo-stand.png"),
        jump_Texture("assets/milo-jump.png"),
        sprite(stand_Texture)
    {
        if (!jumpBuffer.loadFromFile("assets/Sound/jump.wav"))
            throw std::runtime_error("Failed to load jump sound!");
        jumpSound = new sf::Sound(jumpBuffer);
        sprite.setPosition({ 100.f, 500.f });
        sprite.setScale({ 1.f, 1.f });
    }
    ~Player() = default;

    // Get the player sprite
    sf::Sprite& getSprite() {
        return sprite;
    }

    void jump() {
		jumpSound->play(); // Play jump sound
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
        else if (sprite.getPosition().y + sprite.getGlobalBounds().size.y > 600) {
            if (veolocityY > 0.f) {
                sprite.setPosition({ sprite.getPosition().x, 600.f - sprite.getGlobalBounds().size.y });
                veolocityY = 0.f;
            }
        }
        veolocityY += gravity; // Apply gravity to the vertical velocity
        sprite.move({ 0.f, veolocityY });
    }
};

#endif
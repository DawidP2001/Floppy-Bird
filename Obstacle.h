#pragma once
#ifndef OBSTACLE_H
#define OBSTACLE_H

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
        bottomRectangle({ width,  static_cast<float>(height) }),
        texture("assets/Krita-files/100x150-BrickWall.png"),
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

        if (topRectangle.getPosition().x < -width) {
            gapPosition = rand() % (height - gapHeight); // Generate new gap position
            topRectangle.setPosition({ 800.f, 0.f });
            topRectangle.setSize({ 100.f, static_cast<float>(gapPosition) });
            bottomRectangle.setPosition({ 800.f, static_cast<float>(gapPosition + gapHeight) });
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

#endif
#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "Constants.h"

namespace SnakeGame
{
    struct Apple
    {
        int gridX;
        int gridY;
        sf::Sprite sprite;
    };

    void InitApple(Apple& apple, const sf::Texture& texture);
    void PositionApple(Apple& apple);
    void DrawApple(Apple& apple, sf::RenderWindow& window);
    bool CheckAppleCollision(const Apple& apple, const struct Snake& snake);
}
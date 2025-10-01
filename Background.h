#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "Constants.h"

namespace SnakeGame
{
    struct Background
    {
        Position2D position;
        sf::RectangleShape shape;
    };

    void InitBackground(Background& background);
    void DrawBackground(Background& background, sf::RenderWindow& window);
    void SetBackgroundColor(Background& background, sf::Color color);
}
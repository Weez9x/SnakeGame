#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "Constants.h"
#include <vector>

namespace SnakeGame
{
    struct Stone
    {
        int gridX;
        int gridY;
        sf::Sprite sprite;
    };

    void InitStones(std::vector<Stone>& stones, const sf::Texture& texture, int count);
    void PositionStones(std::vector<Stone>& stones, const struct Snake& snake);
    void DrawStones(const std::vector<Stone>& stones, sf::RenderWindow& window);
    bool CheckStoneCollision(const std::vector<Stone>& stones, const struct Snake& snake);
}
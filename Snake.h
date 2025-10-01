#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "Constants.h"
#include <vector>
#include <queue>

namespace SnakeGame
{
    enum SnakeDirection
    {
        Right,
        Up,
        Left,
        Down
    };

    struct SnakeSegments
    {
        int gridX;
        int gridY;
        sf::Sprite sprite;
    };

    struct Snake
    {
        std::vector<SnakeSegments> segments;
        SnakeDirection direction;
        std::queue<SnakeDirection> directionQueue;
        float moveTimer;
        float moveDelay;
    };

    void IniteSnake(Snake& snake, const struct Game& game);
    void GrowSnake(Snake& snake, const struct Game& game);
    void UpdateSnake(Snake& snake, float deltatime);
    void DrawSnake(Snake& snake, sf::RenderWindow& window);
}
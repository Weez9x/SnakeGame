#include "Apple.h"
#include "Game.h"
#include <cstdlib>
#include <ctime>

namespace SnakeGame
{
    void InitApple(Apple& apple, const sf::Texture& texture)
    {
        apple.sprite.setTexture(texture);
        SetSpriteSize(apple.sprite, SIZE_ITEM, SIZE_ITEM);
        SetSpriteRelativeOrigin(apple.sprite, 0.5f, 0.5f);
    }

    void PositionApple(Apple& apple)
    {
        apple.gridX = rand() % GRID_WIDTH;
        apple.gridY = rand() % GRID_HEIGHT;

        float pixelX = apple.gridX * SIZE_ITEM + SIZE_ITEM / 2;
        float pixelY = apple.gridY * SIZE_ITEM + SIZE_ITEM / 2;
        apple.sprite.setPosition(pixelX, pixelY);
    }

    void DrawApple(Apple& apple, sf::RenderWindow& window)
    {
        window.draw(apple.sprite);
    }

    bool CheckAppleCollision(const Apple& apple, const Snake& snake)
    {
        if (snake.segments.empty()) return false;

        const auto& head = snake.segments[0];
        return (head.gridX == apple.gridX && head.gridY == apple.gridY);
    }
}
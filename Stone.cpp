#include "Stone.h"
#include "Game.h"
#include <cstdlib>
#include <ctime>

namespace SnakeGame
{
    void InitStones(std::vector<Stone>& stones, const sf::Texture& texture, int count)
    {
        stones.clear();
        stones.resize(count);

        for (auto& stone : stones)
        {
            stone.sprite.setTexture(texture);
            SetSpriteSize(stone.sprite, SIZE_ITEM, SIZE_ITEM);
            SetSpriteRelativeOrigin(stone.sprite, 0.5f, 0.5f);
        }
    }

    void PositionStones(std::vector<Stone>& stones, const Snake& snake)
    {
        for (auto& stone : stones)
        {
            bool validPosition = false;
            while (!validPosition)
            {
                stone.gridX = rand() % GRID_WIDTH;
                stone.gridY = rand() % GRID_HEIGHT;

                validPosition = true;
                for (const auto& segment : snake.segments)
                {
                    if (segment.gridX == stone.gridX && segment.gridY == stone.gridY)
                    {
                        validPosition = false;
                        break;
                    }
                }

                if (validPosition)
                {
                    for (const auto& otherStone : stones)
                    {
                        if (&stone != &otherStone &&
                            stone.gridX == otherStone.gridX &&
                            stone.gridY == otherStone.gridY)
                        {
                            validPosition = false;
                            break;
                        }
                    }
                }
            }

            float pixelX = stone.gridX * SIZE_ITEM + SIZE_ITEM / 2;
            float pixelY = stone.gridY * SIZE_ITEM + SIZE_ITEM / 2;
            stone.sprite.setPosition(pixelX, pixelY);
        }
    }

    void DrawStones(const std::vector<Stone>& stones, sf::RenderWindow& window)
    {
        for (const auto& stone : stones)
        {
            window.draw(stone.sprite);
        }
    }

    bool CheckStoneCollision(const std::vector<Stone>& stones, const Snake& snake)
    {
        if (snake.segments.empty()) return false;

        const auto& head = snake.segments[0];
        for (const auto& stone : stones)
        {
            if (head.gridX == stone.gridX && head.gridY == stone.gridY)
            {
                return true;
            }
        }
        return false;
    }
}
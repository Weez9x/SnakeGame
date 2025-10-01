#include "Snake.h"
#include "Game.h"

namespace SnakeGame
{
    void IniteSnake(Snake& snake, const Game& game)
    {
        snake.segments.clear();
        snake.direction = SnakeDirection::Right;

        while (!snake.directionQueue.empty())
        {
            snake.directionQueue.pop();
        }

        snake.directionQueue.push(SnakeDirection::Right);

        snake.moveTimer = 0.f;
        snake.moveDelay = 0.1f;

        // Create Head
        SnakeSegments head;
        head.gridX = GRID_WIDTH / 2;
        head.gridY = GRID_HEIGHT / 2;
        head.sprite.setTexture(game.textureHead);
        SetSpriteSize(head.sprite, SIZE_ITEM, SIZE_ITEM);
        SetSpriteRelativeOrigin(head.sprite, 0.5f, 0.5f);
        snake.segments.push_back(head);

        // Create body segments
        for (int i = 0; i < 2; i++)
        {
            SnakeSegments body;
            body.gridX = head.gridX - (i + 1);
            body.gridY = head.gridY;
            body.sprite.setTexture(game.textureBody);
            SetSpriteSize(body.sprite, SIZE_ITEM, SIZE_ITEM);
            SetSpriteRelativeOrigin(body.sprite, 0.5f, 0.5f);
            snake.segments.push_back(body);
        }
    }

    void UpdateSnake(Snake& snake, float deltatime)
    {
        if (snake.segments.empty()) return;

        snake.moveTimer += deltatime;
        if (snake.moveTimer < snake.moveDelay) return;

        snake.moveTimer = 0.f;

        if (!snake.directionQueue.empty())
        {
            snake.direction = snake.directionQueue.front();
            snake.directionQueue.pop();
        }

        // save old position
        std::vector<std::pair<int, int>> oldPositions;
        for (const auto& segment : snake.segments)
        {
            oldPositions.push_back({ segment.gridX, segment.gridY });
        }

        // move head
        switch (snake.direction)
        {
        case SnakeDirection::Right: snake.segments[0].gridX++; break;
        case SnakeDirection::Left:  snake.segments[0].gridX--; break;
        case SnakeDirection::Up:    snake.segments[0].gridY--; break;
        case SnakeDirection::Down:  snake.segments[0].gridY++; break;
        }

        //move body
        for (size_t i = 1; i < snake.segments.size(); i++)
        {
            snake.segments[i].gridX = oldPositions[i - 1].first;
            snake.segments[i].gridY = oldPositions[i - 1].second;
        }
    }

    void DrawSnake(Snake& snake, sf::RenderWindow& window)
    {
        for (size_t i = 0; i < snake.segments.size(); i++)
        {
            float pixelX = snake.segments[i].gridX * SIZE_ITEM + SIZE_ITEM / 2;
            float pixelY = snake.segments[i].gridY * SIZE_ITEM + SIZE_ITEM / 2;

            if (i == 0)
            {
               
                switch (snake.direction)
                {
                case SnakeDirection::Up:    snake.segments[i].sprite.setRotation(-90.f); break;
                case SnakeDirection::Right: snake.segments[i].sprite.setRotation(0.f);   break;
                case SnakeDirection::Down:  snake.segments[i].sprite.setRotation(90.f);  break;
                case SnakeDirection::Left:  snake.segments[i].sprite.setRotation(180.f); break;
                }
            }
            else
            {
                snake.segments[i].sprite.setRotation(0.f);
            }

            snake.segments[i].sprite.setPosition(pixelX, pixelY);
            window.draw(snake.segments[i].sprite);
        }
    }

    void GrowSnake(Snake& snake, const Game& game)
    {
        if (snake.segments.empty()) return;

        // new segment
        SnakeSegments newSegment;
        SnakeSegments& tail = snake.segments.back();

        newSegment.gridX = tail.gridX;
        newSegment.gridY = tail.gridY;
        newSegment.sprite.setTexture(game.textureBody);
        SetSpriteSize(newSegment.sprite, SIZE_ITEM, SIZE_ITEM);
        SetSpriteRelativeOrigin(newSegment.sprite, 0.5f, 0.5f);

        snake.segments.push_back(newSegment);
    }
}
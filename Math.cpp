#include <cstdlib>
#include "Math.h"
#include <SFML/Graphics.hpp>

namespace SnakeGame
{
    Position2D GetRandomPositionInScreen(float screenWidth, float screenHeight)
    {
        Position2D result;
        result.x = rand() / (float)RAND_MAX * screenWidth;
        result.y = rand() / (float)RAND_MAX * screenHeight;
        return result;
    }

    bool isRectanglesCollide(Position2D rect1Position, Vector2D rect1Size, Position2D rect2Position, Vector2D rect2Size)
    {
        float deltaX = (float)fabs(rect1Position.x - rect2Position.x);
        float deltaY = (float)fabs(rect1Position.y - rect2Position.y);

        return deltaX <= (rect1Size.x + rect2Size.x) / 2.f && deltaY <= (rect1Size.y + rect2Size.y) / 2.f;
    }

    bool isCirclesCollide(Position2D circle1Position, float circle1Radius, Position2D circle2Position, float circle2Radius)
    {
        float squareDistance = (circle1Position.x - circle2Position.x) * (circle1Position.x - circle2Position.x) + 
                             (circle1Position.y - circle2Position.y) * (circle1Position.y - circle2Position.y);
        float squareReadiusSum = (circle1Radius + circle2Radius) * (circle1Radius + circle2Radius);

        return squareDistance <= squareReadiusSum;
    }

    void SetSpriteSize(sf::Sprite& sprite, float desiredWidth, float desiredHeight)
    {
        sf::FloatRect spriteRect = sprite.getLocalBounds();
        sf::Vector2f scale = { desiredWidth / spriteRect.width, desiredHeight / spriteRect.height };
        sprite.setScale(scale);
    }

    void SetSpriteRelativeOrigin(sf::Sprite& sprite, float originX, float originY)
    {
        sf::FloatRect spriteRect = sprite.getLocalBounds();
        sf::Vector2f relativeOrigin = { originX * spriteRect.width, originY * spriteRect.height };
        sprite.setOrigin(relativeOrigin);
    }

    void SetTextRelativeOrigin(sf::Text& text, float originX, float originY)
    {
        sf::FloatRect textRect = text.getLocalBounds();
        sf::Vector2f relativeOrigin = { (textRect.left + textRect.width) * originX, 
                                       (textRect.top + textRect.height) * originY };
        text.setOrigin(relativeOrigin);
    }
}
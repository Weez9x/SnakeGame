#include "Background.h"
#include "Game.h"

namespace SnakeGame
{
    void InitBackground(Background& background)
    {
        background.shape.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        SetBackgroundColor(background, sf::Color::Black);
        background.shape.setPosition(0.f, 0.f);
    }

    void DrawBackground(Background& background, sf::RenderWindow& window)
    {
        window.draw(background.shape);
    }

    void SetBackgroundColor(Background& background, const sf::Color color)
    {
        background.shape.setFillColor(color);
    }
}
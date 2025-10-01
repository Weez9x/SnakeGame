#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Game.h"

void HandleWindowEvents(sf::RenderWindow& window, SnakeGame::Game& game)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        // Handle window resize
        if (event.type == sf::Event::Resized)
        {
            // Update the view to the new size of the window
            sf::FloatRect visibleArea(0, 0, (float)event.size.width, (float)event.size.height);
            window.setView(sf::View(visibleArea));
        }
    }
}

int main()
{
    using namespace SnakeGame;

    // Init random seed
    srand((unsigned int)time(nullptr));

    // Init window
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Snake Game!");
    window.setFramerateLimit(60);

    Game game;
    IniteGame(game);

    // Init game clock
    sf::Clock gameClock;
    float lastTime = gameClock.getElapsedTime().asSeconds();

    // Main loop
    while (window.isOpen())
    {
        // Check exit 
        if (game.shouldExit)
        {
            window.close();
            break;
        }

        // Calculate delta time
        float currentTime = gameClock.getElapsedTime().asSeconds();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Handle window events
        HandleWindowEvents(window, game);

        // Update game
        UpdateGame(game, deltaTime);

        // Draw game
        window.clear();
        DrawGame(game, window);
        window.display();
    }

    return 0;
}
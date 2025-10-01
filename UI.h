#pragma once
#include "SFML/Graphics.hpp"
#include <string>
#include <vector>

namespace SnakeGame
{
    struct UIState
    {
        // game
        sf::Text scoreText;

        // main menu (
        sf::Text startGame;
        sf::Text mode;
        sf::Text scoreTable;
        sf::Text settings;
        sf::Text exit;
        sf::Text menuPointer;

        // game mode menu
        sf::Text wallOptions;
        sf::Text stoneOptions;
        sf::Text speedOptions;
        sf::Text modePointer;

        // settings menu 
        sf::Text musicOptions;
        sf::Text soundOptions;
        sf::Text soundVolumeOptions;
        sf::Text settingsPointer;

        // sound volume settings
        sf::Text musicVolumeText;
        sf::Text soundVolumeText;
        sf::Text volumeValueText;
        sf::Text backButton;

        // score table
        sf::Text highScoreTitle;
        std::vector<sf::Text> highScoreEntries;

        // game over
        sf::Text gameOverText;
        sf::Text finalScoreText;
        sf::Text pressAnyKeyText;

        // enter name
        sf::Text enterNameText;
        sf::Text playerNameText;

        sf::Text offPoint;
        sf::Text onPoint;
        // exit
        sf::Text confirmText;
        sf::Text instructionText;

        int currentMenuItem = 0;
        int menuItemsCount = 5; 
        int currentSettingsItem = 0;
        int settingsItemsCount = 3;
    };

    void InitialText(sf::Text& text, const sf::Font& font, unsigned int size, const sf::Color& color, const std::string& str = "", float originX = 0.f, float originY = 0.f, sf::Text::Style style = sf::Text::Regular);
    void InitUI(UIState& uiState, const sf::Font& standartFont);
    void UpdateUI(UIState& uiState, const struct Game& game, const sf::Font& standartFont);
    void DrawUI(UIState& uiState, const struct Game& game, sf::RenderWindow& window);
}
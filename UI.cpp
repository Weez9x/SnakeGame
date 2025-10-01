#include "UI.h"
#include "Game.h"
#include <random>
#include <sstream>
#include <iomanip>

namespace SnakeGame
{
    void InitialText(sf::Text& text, const sf::Font& font, unsigned int size, const sf::Color& color, const std::string& str, float originX, float originY, sf::Text::Style style)
    {
        text.setFont(font);
        text.setCharacterSize(size);
        text.setFillColor(color);
        text.setStyle(style);
        if (!str.empty())
        {
            text.setString(str);
        }
        if (originX != 0.f || originY != 0.f)
        {
            SetTextRelativeOrigin(text, originX, originY);
        }
    }

    void InitUI(UIState& uiState, const sf::Font& standartFont)
    {
        // PlayGame
        InitialText(uiState.scoreText, standartFont, 24, sf::Color::White, "Score: 0", 0.0f, 0.f);

        // MainMenu
        InitialText(uiState.startGame, standartFont, 30, sf::Color::White, "Play", 0.5f, 0.5f);
        InitialText(uiState.mode, standartFont, 30, sf::Color::White, "Game Mode", 0.5f, 0.5f);
        InitialText(uiState.scoreTable, standartFont, 30, sf::Color::White, "Table of Records", 0.5f, 0.5f);
        InitialText(uiState.settings, standartFont, 30, sf::Color::White, "Options", 0.5f, 0.5f);
        InitialText(uiState.exit, standartFont, 30, sf::Color::White, "Exit", 0.5f, 0.5f);
        InitialText(uiState.menuPointer, standartFont, 30, sf::Color::Red, ">", 0.5f, 0.5f);

        uiState.menuItemsCount = 5;

        // GameModeMenu
        InitialText(uiState.speedOptions, standartFont, 30, sf::Color::White, "Acceleration: ", 0.5f, 0.5f);
        InitialText(uiState.wallOptions, standartFont, 30, sf::Color::White, "Walls: ", 0.5f, 0.5f);
        InitialText(uiState.stoneOptions, standartFont, 30, sf::Color::White, "Stones: ", 0.5f, 0.5f);
        InitialText(uiState.modePointer, standartFont, 30, sf::Color::Red, ">", 0.5f, 0.5f);

        // Settings menu 
        InitialText(uiState.musicOptions, standartFont, 30, sf::Color::White, "Music: ", 0.5f, 0.5f);
        InitialText(uiState.soundOptions, standartFont, 30, sf::Color::White, "Sound Effects: ", 0.5f, 0.5f);
        InitialText(uiState.soundVolumeOptions, standartFont, 30, sf::Color::White, "Sound Volume", 0.5f, 0.5f);
        InitialText(uiState.settingsPointer, standartFont, 30, sf::Color::Red, ">", 0.5f, 0.5f);

        uiState.settingsItemsCount = 3;

        // Sound volume settings
        InitialText(uiState.musicVolumeText, standartFont, 30, sf::Color::White, "Music Volume: ", 0.5f, 0.5f);
        InitialText(uiState.soundVolumeText, standartFont, 30, sf::Color::White, "Sound Volume: ", 0.5f, 0.5f);
        InitialText(uiState.volumeValueText, standartFont, 24, sf::Color::Yellow, "100", 0.5f, 0.5f);
        InitialText(uiState.backButton, standartFont, 30, sf::Color::White, "Back", 0.5f, 0.5f);

        // Score table
        InitialText(uiState.highScoreTitle, standartFont, 40, sf::Color::Yellow, "HIGH SCORES", 0.5f, 0.5f);
        uiState.highScoreEntries.resize(MAX_HIGH_SCORES);
        for (int i = 0; i < MAX_HIGH_SCORES; ++i)
        {
            InitialText(uiState.highScoreEntries[i], standartFont, 24, sf::Color::White, "", 0.5f, 0.5f);
        }

        // Game over
        InitialText(uiState.gameOverText, standartFont, 50, sf::Color::Red, "GAME OVER", 0.5f, 0.5f);
        InitialText(uiState.finalScoreText, standartFont, 30, sf::Color::White, "Score: 0", 0.5f, 0.5f);
        InitialText(uiState.pressAnyKeyText, standartFont, 24, sf::Color::Yellow, "Press any key to continue", 0.5f, 0.5f);

        // Enter name
        InitialText(uiState.enterNameText, standartFont, 40, sf::Color::Yellow, "ENTER YOUR NAME", 0.5f, 0.5f);
        InitialText(uiState.playerNameText, standartFont, 36, sf::Color::White, "PLAYER", 0.5f, 0.5f);

        InitialText(uiState.offPoint, standartFont, 30, sf::Color::White, "Off", 0.5f, 0.5f);
        InitialText(uiState.onPoint, standartFont, 30, sf::Color::White, "On", 0.5f, 0.5f);

        InitialText(uiState.confirmText, standartFont, 36, sf::Color::White, "Are you sure you want to exit?", 0.5f, 0.5f);
        InitialText(uiState.instructionText, standartFont, 24, sf::Color::Yellow,"Press ENTER to confirm, ESC to cancel", 0.5f, 0.5f);
    }

    void UpdateUI(UIState& uiState, const Game& game, const sf::Font& standartFont)
    {
        switch (game.stateGame)
        {
        case MainMenu:
        {
            float pointerY = 195.f + uiState.currentMenuItem * 50.f;
            uiState.menuPointer.setPosition(250.f, pointerY);

            uiState.startGame.setFillColor(sf::Color::White);
            uiState.mode.setFillColor(sf::Color::White);
            uiState.scoreTable.setFillColor(sf::Color::White);
            uiState.settings.setFillColor(sf::Color::White);
            uiState.exit.setFillColor(sf::Color::White);

            switch (uiState.currentMenuItem)
            {
            case 0: uiState.startGame.setFillColor(sf::Color::Red); break;
            case 1: uiState.mode.setFillColor(sf::Color::Red); break;
            case 2: uiState.scoreTable.setFillColor(sf::Color::Red); break;
            case 3: uiState.settings.setFillColor(sf::Color::Red); break;
            case 4: uiState.exit.setFillColor(sf::Color::Red); break;
            }
            break;
        }
        case GameMode:
        {
            float pointerY = 195.f + uiState.currentMenuItem * 50.f;
            uiState.modePointer.setPosition(250.f, pointerY);
            break;
        }
        case GameSettingsState:
        {
            float pointerY = 195.f + uiState.currentSettingsItem * 50.f;
            uiState.settingsPointer.setPosition(250.f, pointerY);
            break;
        }
        case SoundSettings:
        {
            float pointerY = 195.f + uiState.currentSettingsItem * 50.f;
            uiState.settingsPointer.setPosition(250.f, pointerY);

            if (uiState.currentSettingsItem == 0)
            {
                uiState.volumeValueText.setString(std::to_string(game.settings.musicVolume));
            }
            else
            {
                uiState.volumeValueText.setString(std::to_string(game.settings.soundVolume));
            }
            break;
        }
        case PlayGame:
        {
            uiState.scoreText.setString("Score: " + std::to_string(game.score));
            break;
        }
        case ScoreTable:
        {
            // Обновление таблицы рекордов
            for (int i = 0; i < MAX_HIGH_SCORES && i < game.highScores.size(); ++i)
            {
                const auto& entry = game.highScores[i];
                std::stringstream ss;
                ss << (i + 1) << ". " << entry.playerName << " - " << entry.score;
                uiState.highScoreEntries[i].setString(ss.str());
            }
            break;
        }
        case GameOver:
        {
            uiState.finalScoreText.setString("Score: " + std::to_string(game.score));
            break;
        }
        case EnterName:
        {
            uiState.playerNameText.setString(game.currentPlayerName);
            SetTextRelativeOrigin(uiState.playerNameText, 0.5f, 0.5f);
            break;
        }
        }
    }

    void DrawUI(UIState& uiState, const Game& game, sf::RenderWindow& window)
    {
        switch (game.stateGame)
        {
        case MainMenu:
        {
            uiState.startGame.setPosition(400.f, 200.f);
            uiState.mode.setPosition(400.f, 250.f);
            uiState.scoreTable.setPosition(400.f, 300.f);
            uiState.settings.setPosition(400.f, 350.f);
            uiState.exit.setPosition(400.f, 400.f);

            window.draw(uiState.startGame);
            window.draw(uiState.mode);
            window.draw(uiState.scoreTable);
            window.draw(uiState.settings);
            window.draw(uiState.exit);
            window.draw(uiState.menuPointer);
            break;
        }
        case GameMode:
        {
            uiState.speedOptions.setPosition(400.f, 200.f);
            uiState.wallOptions.setPosition(400.f, 250.f);
            uiState.stoneOptions.setPosition(400.f, 300.f);

            sf::Text speedStatus, wallStatus, stoneStatus;
            InitialText(speedStatus, game.standartFont, 30,
                game.settings.accelerationEnabled ? sf::Color::Green : sf::Color::Red,
                game.settings.accelerationEnabled ? "On" : "Off", 0.5f, 0.5f);
            InitialText(wallStatus, game.standartFont, 30,
                game.settings.wallsEnabled ? sf::Color::Green : sf::Color::Red,
                game.settings.wallsEnabled ? "On" : "Off", 0.5f, 0.5f);
            InitialText(stoneStatus, game.standartFont, 30,
                game.settings.stonesEnabled ? sf::Color::Green : sf::Color::Red,
                game.settings.stonesEnabled ? "On" : "Off", 0.5f, 0.5f);

            speedStatus.setPosition(600.f, 200.f);
            wallStatus.setPosition(600.f, 250.f);
            stoneStatus.setPosition(600.f, 300.f);

            window.draw(uiState.speedOptions);
            window.draw(uiState.wallOptions);
            window.draw(uiState.stoneOptions);
            window.draw(speedStatus);
            window.draw(wallStatus);
            window.draw(stoneStatus);
            window.draw(uiState.modePointer);
            break;
        }
        case GameSettingsState:
        {
            uiState.musicOptions.setPosition(400.f, 200.f);
            uiState.soundOptions.setPosition(400.f, 250.f);
            uiState.soundVolumeOptions.setPosition(400.f, 300.f);

            sf::Text musicStatus, soundStatus;
            InitialText(musicStatus, game.standartFont, 30,
                game.settings.musicEnabled ? sf::Color::Green : sf::Color::Red,
                game.settings.musicEnabled ? "On" : "Off", 0.5f, 0.5f);
            InitialText(soundStatus, game.standartFont, 30,
                game.settings.soundEnabled ? sf::Color::Green : sf::Color::Red,
                game.settings.soundEnabled ? "On" : "Off", 0.5f, 0.5f);

            musicStatus.setPosition(600.f, 200.f);
            soundStatus.setPosition(600.f, 250.f);

            window.draw(uiState.musicOptions);
            window.draw(uiState.soundOptions);
            window.draw(uiState.soundVolumeOptions);
            window.draw(musicStatus);
            window.draw(soundStatus);
            window.draw(uiState.settingsPointer);
            break;
        }
        case SoundSettings:
        {
            uiState.musicVolumeText.setPosition(400.f, 200.f);
            uiState.soundVolumeText.setPosition(400.f, 250.f);
            uiState.backButton.setPosition(400.f, 300.f);

            uiState.volumeValueText.setPosition(600.f, 200.f + uiState.currentSettingsItem * 50.f);

            window.draw(uiState.musicVolumeText);
            window.draw(uiState.soundVolumeText);
            window.draw(uiState.backButton);
            window.draw(uiState.volumeValueText);
            window.draw(uiState.settingsPointer);
            break;
        }
        case PlayGame:
        {
            uiState.scoreText.setPosition(10.f, 10.f);
            window.draw(uiState.scoreText);
            break;
        }
        case ScoreTable:
        {
            uiState.highScoreTitle.setPosition(400.f, 80.f);
            window.draw(uiState.highScoreTitle);

            for (int i = 0; i < MAX_HIGH_SCORES && i < game.highScores.size(); ++i)
            {
                uiState.highScoreEntries[i].setPosition(400.f, 150.f + i * 40.f);
                window.draw(uiState.highScoreEntries[i]);
            }

 
            uiState.backButton.setPosition(400.f, 550.f);
            window.draw(uiState.backButton);
            break;
        }
        case GameOver:
        {
            uiState.gameOverText.setPosition(400.f, 200.f);
            uiState.finalScoreText.setPosition(400.f, 280.f);
            uiState.pressAnyKeyText.setPosition(400.f, 350.f);

            window.draw(uiState.gameOverText);
            window.draw(uiState.finalScoreText);
            window.draw(uiState.pressAnyKeyText);
            break;
        }
        case EnterName:
        {
            uiState.enterNameText.setPosition(400.f, 200.f);
            uiState.playerNameText.setPosition(400.f, 280.f);

            window.draw(uiState.enterNameText);
            window.draw(uiState.playerNameText);
            break;
        }
        case ExitConfirm:
        {
            uiState.confirmText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50);
            uiState.instructionText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 20);

            window.draw(uiState.confirmText);
            window.draw(uiState.instructionText);
        }
        }
    }
}
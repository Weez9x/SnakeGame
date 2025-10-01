#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Math.h"
#include "Constants.h"
#include "Snake.h"
#include "Apple.h"
#include "Stone.h"
#include "UI.h"
#include "Background.h"
#include <vector>
#include <string>

namespace SnakeGame
{
    struct GameSettings
    {
        bool musicEnabled = true;
        bool soundEnabled = true;
        int musicVolume = 100;
        int soundVolume = 100;
        bool accelerationEnabled = false;
        bool wallsEnabled = false;
        bool stonesEnabled = false;
    };

    struct HighScoreEntry
    {
        std::string playerName;
        int score;
        time_t date;
    };

    enum GameState
    {
        MainMenu,
        GameMode,
        PlayGame,
        GameSettingsState,
        SoundSettings,
        ScoreTable,
        GameOver,
        EnterName,
        ExitConfirm
    };

    struct Game
    {
        // Textures
        sf::Texture textureHead;
        sf::Texture textureBody;
        sf::Texture textureApple;
        sf::Texture textureStone;

        // Font
        sf::Font standartFont;

        // Audio
        sf::Music backgroundMusic;
        sf::SoundBuffer eatSoundBuffer;
        sf::Sound eatSound;
        sf::SoundBuffer crashSoundBuffer;
        sf::Sound crashSound;
        sf::SoundBuffer menuMoveSoundBuffer;
        sf::Sound menuMoveSound;
        sf::SoundBuffer menuSelectSoundBuffer;
        sf::Sound menuSelectSound;

        // Game objects
        Snake snake;
        Apple apple;
        std::vector<Stone> stones;
        Background background;
        UIState uiState;
        GameSettings settings;

        // Game state
        GameState stateGame = GameState::MainMenu;
        int score = 0;
        std::vector<HighScoreEntry> highScores;
        std::string currentPlayerName = PLAYER_NAME;

        bool soundPlayedThisFrame = false;
        bool shouldExit = false;
    };

    // Game
    void SaveHighScores(const std::vector<HighScoreEntry>& highScores);
    void LoadHighScores(std::vector<HighScoreEntry>& highScores);
    void SaveSettings(const GameSettings& settings);
    void LoadSettings(GameSettings& settings);
    void AddHighScore(std::vector<HighScoreEntry>& highScores, const std::string& name, int score);
    void UpdateAudioSettings(Game& game);
    void PlayMenuSound(Game& game, bool isMoveSound);
    void RestartGame(Game& game);
    void IniteGame(Game& game);
    void HandlerInput(Game& game);
    void UpdateGame(Game& game, float deltatime);
    void DrawGame(Game& game, sf::RenderWindow& window);

    // Input handlers
    void HandleMainMenuInput(Game& game);
    void HandleGameModeInput(Game& game);
    void HandleGameSettingsInput(Game& game);
    void HandleSoundSettingsInput(Game& game);
    void HandlePlayGameInput(Game& game);
    void HandleScoreTableInput(Game& game);
    void HandleGameOverInput(Game& game);
    void HandleEnterNameInput(Game& game);
    void HandleExitConfirmInput(Game& game);

    // Game logic
    void CheckCollisions(Game& game);
    void SpawnApple(Game& game);
    void SpawnStones(Game& game);
}
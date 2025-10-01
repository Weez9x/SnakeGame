#include "Game.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

namespace SnakeGame
{
    void SaveHighScores(const std::vector<HighScoreEntry>& highScores)
    {
        std::ofstream file(HIGH_SCORES_FILE, std::ios::binary);
        if (file.is_open())
        {
            size_t count = highScores.size();
            file.write(reinterpret_cast<const char*>(&count), sizeof(count));
            for (const auto& entry : highScores)
            {
                size_t nameLength = entry.playerName.size();
                file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
                file.write(entry.playerName.c_str(), nameLength);
                file.write(reinterpret_cast<const char*>(&entry.score), sizeof(entry.score));
                file.write(reinterpret_cast<const char*>(&entry.date), sizeof(entry.date));
            }
        }
    }

    void LoadHighScores(std::vector<HighScoreEntry>& highScores)
    {
        std::ifstream file(HIGH_SCORES_FILE, std::ios::binary);
        if (file.is_open())
        {
            size_t count;
            file.read(reinterpret_cast<char*>(&count), sizeof(count));
            if (count > MAX_HIGH_SCORES) count = MAX_HIGH_SCORES;

            highScores.resize(count);

            for (auto& entry : highScores)
            {
                size_t nameLength;
                file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
                entry.playerName.resize(nameLength);
                file.read(&entry.playerName[0], nameLength);
                file.read(reinterpret_cast<char*>(&entry.score), sizeof(entry.score));
                file.read(reinterpret_cast<char*>(&entry.date), sizeof(entry.date));
            }
        }
    }

    void SaveSettings(const GameSettings& settings)
    {
        std::ofstream file(SETTINGS_FILE, std::ios::binary);
        if (file.is_open())
        {
            file.write(reinterpret_cast<const char*>(&settings), sizeof(settings));
        }
    }

    void LoadSettings(GameSettings& settings)
    {
        std::ifstream file(SETTINGS_FILE, std::ios::binary);
        if (file.is_open())
        {
            file.read(reinterpret_cast<char*>(&settings), sizeof(settings));
        }
    }

    void AddHighScore(std::vector<HighScoreEntry>& highScores, const std::string& name, int score)
    {
        HighScoreEntry newEntry;
        newEntry.playerName = name;
        newEntry.score = score;
        newEntry.date = time(nullptr);

        highScores.push_back(newEntry);
        std::sort(highScores.begin(), highScores.end(),
            [](const HighScoreEntry& a, const HighScoreEntry& b) { return a.score > b.score; });

        if (highScores.size() > MAX_HIGH_SCORES)
        {
            highScores.pop_back();
        }

        SaveHighScores(highScores);
    }

    void UpdateAudioSettings(Game& game)
    {
        if (game.settings.musicEnabled)
        {
            game.backgroundMusic.setVolume((float)game.settings.musicVolume);
            if (game.backgroundMusic.getStatus() != sf::Music::Playing)
            {
                game.backgroundMusic.play();
            }
        }
        else
        {
            game.backgroundMusic.stop();
        }

        game.eatSound.setVolume((float)game.settings.soundVolume);
        game.crashSound.setVolume((float)game.settings.soundVolume);
        game.menuMoveSound.setVolume((float)game.settings.soundVolume);
        game.menuSelectSound.setVolume((float)game.settings.soundVolume);
    }

    void PlayMenuSound(Game& game, bool isMoveSound)
    {
        if (game.settings.soundEnabled && !game.soundPlayedThisFrame)
        {
            if (isMoveSound)
            {
                game.menuMoveSound.play();
            }
            else
            {
                game.menuSelectSound.play();
            }
            game.soundPlayedThisFrame = true;
        }
    }

    void SpawnApple(Game& game)
    {
        PositionApple(game.apple);

        // check that the apple is not on a snake
        bool validPosition = false;
        while (!validPosition)
        {
            validPosition = true;
            for (const auto& segment : game.snake.segments)
            {
                if (segment.gridX == game.apple.gridX && segment.gridY == game.apple.gridY)
                {
                    validPosition = false;
                    PositionApple(game.apple);
                    break;
                }
            }

            // check that the apple is not on the rocks
            if (validPosition && game.settings.stonesEnabled)
            {
                for (const auto& stone : game.stones)
                {
                    if (stone.gridX == game.apple.gridX && stone.gridY == game.apple.gridY)
                    {
                        validPosition = false;
                        PositionApple(game.apple);
                        break;
                    }
                }
            }
        }
    }

    void SpawnStones(Game& game)
    {
        if (game.settings.stonesEnabled)
        {
            InitStones(game.stones, game.textureStone, 5); // 5 камней
            PositionStones(game.stones, game.snake);
        }
        else
        {
            game.stones.clear();
        }
    }

    void RestartGame(Game& game)
    {
        // reinitialize the snake
        IniteSnake(game.snake, game);

        // Reset the count
        game.score = 0;

        // Updating the invoice text display
        game.uiState.scoreText.setString("Score: 0");

        // Reinventing the Apple
        SpawnApple(game);

        // Recreate the stones
        SpawnStones(game);

        // Resetting the game state
        game.stateGame = GameState::MainMenu;

        // Updating audio settings
        UpdateAudioSettings(game);

        // Resetting the player's name
        game.currentPlayerName = PLAYER_NAME;
    }

    void IniteGame(Game& game)
    {
        // Init random
        srand(static_cast<unsigned int>(time(nullptr)));

        // Load textures
        assert(game.textureHead.loadFromFile(RESOURCES_PATH + TEXTURE + "/Snake_head.png"));
        assert(game.textureBody.loadFromFile(RESOURCES_PATH + TEXTURE + "/Snake_body.png"));
        assert(game.textureApple.loadFromFile(RESOURCES_PATH + TEXTURE + "/Apple.png"));
        assert(game.textureStone.loadFromFile(RESOURCES_PATH + TEXTURE + "/Stone.png"));
        assert(game.standartFont.loadFromFile(RESOURCES_PATH + FONT + "/PixelizerBold.ttf"));

        // Load audio
        if (game.backgroundMusic.openFromFile(RESOURCES_PATH + SOUND + "/Clinthammer__Background_Music.wav"))
        {
            game.backgroundMusic.setLoop(true);
        }
        if (game.eatSoundBuffer.loadFromFile(RESOURCES_PATH + SOUND + "/Owlstorm__Snake_hit.wav"))
        {
            game.eatSound.setBuffer(game.eatSoundBuffer);
        }
        if (game.crashSoundBuffer.loadFromFile(RESOURCES_PATH + SOUND + "/Maodin204__Lose.wav"))
        {
            game.crashSound.setBuffer(game.crashSoundBuffer);
        }

        if (game.menuMoveSoundBuffer.loadFromFile(RESOURCES_PATH + SOUND + "/Theevilsocks__menu-hover.wav"))
        {
            game.menuMoveSound.setBuffer(game.menuMoveSoundBuffer);
        }
        if (game.menuSelectSoundBuffer.loadFromFile(RESOURCES_PATH + SOUND + "/Timgormly__Enter.wav"))
        {
            game.menuSelectSound.setBuffer(game.menuSelectSoundBuffer);
        }

        // Load settings and high scores
        LoadSettings(game.settings);
        LoadHighScores(game.highScores);

        // Init game objects
        InitBackground(game.background);
        InitApple(game.apple, game.textureApple);
        InitUI(game.uiState, game.standartFont);

        RestartGame(game);
    }

    void HandleMainMenuInput(Game& game)
    {
        // Reset the audio flag at the start of the frame
        game.soundPlayedThisFrame = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            game.uiState.currentMenuItem = (game.uiState.currentMenuItem + 1) % game.uiState.menuItemsCount;
            PlayMenuSound(game, true); // Sound of movement
            sf::sleep(sf::milliseconds(150));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            game.uiState.currentMenuItem = (game.uiState.currentMenuItem - 1 + game.uiState.menuItemsCount) % game.uiState.menuItemsCount;
            PlayMenuSound(game, true); // Sound of movement
            sf::sleep(sf::milliseconds(150));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            PlayMenuSound(game, false); // Sound of choice

            switch (game.uiState.currentMenuItem)
            {
            case 0:
            {// Play
                RestartGame(game);
                game.stateGame = GameState::PlayGame;
                break;
            }
            case 1: // Game Mode
            {
                game.stateGame = GameState::GameMode;
                game.uiState.currentMenuItem = 0;
                break;
            }
            case 2: // Table of Records
            {
                game.stateGame = GameState::ScoreTable;
                break;
            }
            case 3: // Options
            {
                game.stateGame = GameState::GameSettingsState;
                game.uiState.currentSettingsItem = 0;
                break;
            }
            case 4: // Exit
            {
                game.stateGame = GameState::ExitConfirm; // Переходим к подтверждению
                break;
            }
            }
            sf::sleep(sf::milliseconds(150));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            PlayMenuSound(game, false); 
            sf::sleep(sf::milliseconds(150));
        }
    }

    void HandleGameModeInput(Game& game)
    {
        game.soundPlayedThisFrame = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            game.uiState.currentMenuItem = (game.uiState.currentMenuItem + 1) % 3;
            PlayMenuSound(game, true);
            sf::sleep(sf::milliseconds(150));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            game.uiState.currentMenuItem = (game.uiState.currentMenuItem - 1 + 3) % 3;
            PlayMenuSound(game, true);
            sf::sleep(sf::milliseconds(150));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            PlayMenuSound(game, false);

            switch (game.uiState.currentMenuItem)
            {
            case 0: // Acceleration
            {
                game.settings.accelerationEnabled = !game.settings.accelerationEnabled;
                SaveSettings(game.settings);
                break;
            }
            case 1: // Walls
            {
                game.settings.wallsEnabled = !game.settings.wallsEnabled;
                SaveSettings(game.settings);
                break;
            }
            case 2: // Stones
            {
                game.settings.stonesEnabled = !game.settings.stonesEnabled;
                SaveSettings(game.settings);
                SpawnStones(game);
                break;
            }
            }
            sf::sleep(sf::milliseconds(150));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            PlayMenuSound(game, false);
            game.stateGame = GameState::MainMenu;
            sf::sleep(sf::milliseconds(150));
        }
    }

    void HandleGameSettingsInput(Game& game)
    {
        game.soundPlayedThisFrame = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            game.uiState.currentSettingsItem = (game.uiState.currentSettingsItem + 1) % game.uiState.settingsItemsCount;
            PlayMenuSound(game, true);
            sf::sleep(sf::milliseconds(150));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            game.uiState.currentSettingsItem = (game.uiState.currentSettingsItem - 1 + game.uiState.settingsItemsCount) % game.uiState.settingsItemsCount;
            PlayMenuSound(game, true);
            sf::sleep(sf::milliseconds(150));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            PlayMenuSound(game, false);

            switch (game.uiState.currentSettingsItem)
            {
            case 0:
            {// Music on/off
                game.settings.musicEnabled = !game.settings.musicEnabled;
                UpdateAudioSettings(game);
                SaveSettings(game.settings);
                break;
            }
            case 1: // Sound effects on/off
            {
                game.settings.soundEnabled = !game.settings.soundEnabled;
                SaveSettings(game.settings);
                break;
            }
            case 2: // Sound Volume settings
            {
                game.stateGame = GameState::SoundSettings;
                game.uiState.currentSettingsItem = 0;
                break;
            }
            }
            sf::sleep(sf::milliseconds(150));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            PlayMenuSound(game, false);
            game.stateGame = GameState::MainMenu;
            sf::sleep(sf::milliseconds(150));
        }
    }

    void HandleSoundSettingsInput(Game& game)
        {
            game.soundPlayedThisFrame = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                game.uiState.currentSettingsItem = (game.uiState.currentSettingsItem + 1) % 3;
                PlayMenuSound(game, true);
                sf::sleep(sf::milliseconds(150));
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                game.uiState.currentSettingsItem = (game.uiState.currentSettingsItem - 1 + 3) % 3;
                PlayMenuSound(game, true);
                sf::sleep(sf::milliseconds(150));
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                if (game.uiState.currentSettingsItem == 0) // Music volume
                {
                    game.settings.musicVolume = std::max(0, game.settings.musicVolume - 5);
                }
                else if (game.uiState.currentSettingsItem == 1) // Sound volume
                {
                    game.settings.soundVolume = std::max(0, game.settings.soundVolume - 5);
                }
                PlayMenuSound(game, true);
                UpdateAudioSettings(game);
                SaveSettings(game.settings);
                sf::sleep(sf::milliseconds(100));
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                if (game.uiState.currentSettingsItem == 0) // Music volume
                {
                    game.settings.musicVolume = std::min(100, game.settings.musicVolume + 5);
                }
                else if (game.uiState.currentSettingsItem == 1) // Sound volume
                {
                    game.settings.soundVolume = std::min(100, game.settings.soundVolume + 5);
                }
                PlayMenuSound(game, true);
                UpdateAudioSettings(game);
                SaveSettings(game.settings);
                sf::sleep(sf::milliseconds(100));
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                PlayMenuSound(game, false);
                if (game.uiState.currentSettingsItem == 2) // Back
                {
                    game.stateGame = GameState::GameSettingsState;
                }
                sf::sleep(sf::milliseconds(150));
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                PlayMenuSound(game, false);
                game.stateGame = GameState::GameSettingsState;
                sf::sleep(sf::milliseconds(150));
            }
        }

    void HandlePlayGameInput(Game& game)
    {
        if (game.snake.directionQueue.size() < 2)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                if (game.snake.direction != SnakeDirection::Left)
                {
                    if (game.snake.directionQueue.empty() || game.snake.directionQueue.back() != SnakeDirection::Right)
                    {
                        game.snake.directionQueue.push(SnakeDirection::Right);
                    }
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                if (game.snake.direction != SnakeDirection::Down)
                {
                    if (game.snake.directionQueue.empty() || game.snake.directionQueue.back() != SnakeDirection::Up)
                    {
                        game.snake.directionQueue.push(SnakeDirection::Up);
                    }
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                if (game.snake.direction != SnakeDirection::Right)
                {
                    if (game.snake.directionQueue.empty() || game.snake.directionQueue.back() != SnakeDirection::Left)
                    {
                        game.snake.directionQueue.push(SnakeDirection::Left);
                    }
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                if (game.snake.direction != SnakeDirection::Up)
                {
                    if (game.snake.directionQueue.empty() || game.snake.directionQueue.back() != SnakeDirection::Down)
                    {
                        game.snake.directionQueue.push(SnakeDirection::Down);
                    }
                }
            }
        }

        // Escape to return to main menu
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            game.stateGame = GameState::MainMenu;
            sf::sleep(sf::milliseconds(150));
        }
    }

    void HandleScoreTableInput(Game& game)
    {
        game.soundPlayedThisFrame = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            PlayMenuSound(game, false);
            game.stateGame = GameState::MainMenu;
            sf::sleep(sf::milliseconds(150));
        }
    }

    void HandleGameOverInput(Game& game)
    {
        game.soundPlayedThisFrame = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            PlayMenuSound(game, false);

            // Check if score is high enough for high scores
            if (game.highScores.size() < MAX_HIGH_SCORES || (!game.highScores.empty() && game.score > game.highScores.back().score))
            {
                game.stateGame = GameState::EnterName;
                game.currentPlayerName = "";
            }
            else
            {
                game.stateGame = GameState::MainMenu;
            }
            sf::sleep(sf::milliseconds(150));
        }
    }

    void HandleEnterNameInput(Game& game)
    {
        game.soundPlayedThisFrame = false;

        // Handle backspace
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
        {
            if (!game.currentPlayerName.empty())
            {
                game.currentPlayerName.pop_back();
                PlayMenuSound(game, true);
            }
            sf::sleep(sf::milliseconds(100));
        }
        // Handle enter to confirm name
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            if (!game.currentPlayerName.empty())
            {
                PlayMenuSound(game, false);
                AddHighScore(game.highScores, game.currentPlayerName, game.score);
                game.stateGame = GameState::MainMenu;
            }
            sf::sleep(sf::milliseconds(150));
        }
        // Handle escape to cancel
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            PlayMenuSound(game, false);
            game.stateGame = GameState::MainMenu;
            sf::sleep(sf::milliseconds(150));
        }
        // Handle letter input
        else
        {
            // A-Z
            for (int i = 0; i < 26; ++i)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(sf::Keyboard::A + i)))
                {
                    if (game.currentPlayerName.length() < 15)
                    {
                        char letter = 'A' + i;
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
                        {
                            letter = 'a' + i;
                        }
                        game.currentPlayerName += letter;
                        PlayMenuSound(game, true);
                    }
                    sf::sleep(sf::milliseconds(100));
                    break;
                }
            }

            // Space
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                if (game.currentPlayerName.length() < 15)
                {
                    game.currentPlayerName += ' ';
                    PlayMenuSound(game, true);
                }
                sf::sleep(sf::milliseconds(100));
            }
        }
    }

    void HandleExitConfirmInput(Game& game)
    {
        game.soundPlayedThisFrame = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            PlayMenuSound(game, false);
            game.shouldExit = true; // Confirm exit
            sf::sleep(sf::milliseconds(150));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            PlayMenuSound(game, false);
            game.stateGame = GameState::MainMenu; // Cancel exit
            sf::sleep(sf::milliseconds(150));
        }
    }

    void HandlerInput(Game& game)
    {
        switch (game.stateGame)
        {
        case MainMenu: HandleMainMenuInput(game); break;
        case GameMode: HandleGameModeInput(game); break;
        case GameSettingsState: HandleGameSettingsInput(game); break;
        case SoundSettings: HandleSoundSettingsInput(game); break;
        case PlayGame: HandlePlayGameInput(game); break;
        case ScoreTable: HandleScoreTableInput(game); break;
        case GameOver: HandleGameOverInput(game); break;
        case EnterName: HandleEnterNameInput(game); break;
        case ExitConfirm: HandleExitConfirmInput(game); break;
        }
    }

    void CheckCollisions(Game& game)
    {
        if (game.stateGame == GameState::GameOver) return;

        if (game.snake.segments.empty()) return;

        const auto& head = game.snake.segments[0];

        // Check wall collision if walls are enabled
        if (game.settings.wallsEnabled)
        {
            if (head.gridX < 0 || head.gridX >= GRID_WIDTH || head.gridY < 0 || head.gridY >= GRID_HEIGHT)
            {
                // Game over - wall collision
                if (game.settings.soundEnabled)
                {
                    game.crashSound.play();
                }
                game.stateGame = GameState::GameOver;
                return;
            }
        }
        else
        {
            // teleportation across borders
            if (head.gridX < 0) game.snake.segments[0].gridX = GRID_WIDTH - 1;
            else if (head.gridX >= GRID_WIDTH) game.snake.segments[0].gridX = 0;

            if (head.gridY < 0) game.snake.segments[0].gridY = GRID_HEIGHT - 1;
            else if (head.gridY >= GRID_HEIGHT) game.snake.segments[0].gridY = 0;
        }

        // Check apple collision
        if (CheckAppleCollision(game.apple, game.snake))
        {
            if (game.settings.soundEnabled)
            {
                game.eatSound.play();
            }

            GrowSnake(game.snake, game);
            game.score += 10;
            SpawnApple(game);
        }

        // Check stone collision
        if (game.settings.stonesEnabled && CheckStoneCollision(game.stones, game.snake))
        {
            if (game.settings.soundEnabled)
            {
                game.crashSound.play();
            }
            game.stateGame = GameState::GameOver;
            return;
        }

        // Check self collision
        for (size_t i = 1; i < game.snake.segments.size(); ++i)
        {
            const auto& segment = game.snake.segments[i];
            if (head.gridX == segment.gridX && head.gridY == segment.gridY)
            {
                // Game over - self collision
                if (game.settings.soundEnabled)
                {
                    game.crashSound.play();
                }
                game.stateGame = GameState::GameOver;
                return;
            }
        }
    }

    void UpdateGame(Game& game, float deltaTime)
    {
        HandlerInput(game);
        UpdateUI(game.uiState, game, game.standartFont);

        if (game.stateGame == PlayGame)
        {
            UpdateSnake(game.snake, deltaTime);
            CheckCollisions(game);

            // Update snake speed based on acceleration setting
            if (game.settings.accelerationEnabled)
            {
                // Increase speed as snake grows
                float baseSpeed = 0.1f;
                float speedIncrease = game.snake.segments.size() * 0.005f;
                game.snake.moveDelay = std::max(0.05f, baseSpeed - speedIncrease);
            }
            else
            {
                game.snake.moveDelay = 0.1f; // Fixed speed
            }
        }
    }

    void DrawGame(Game& game, sf::RenderWindow& window)
    {
        DrawBackground(game.background, window);

        if (game.settings.wallsEnabled)
        {
            sf::RectangleShape wall(sf::Vector2f(SCREEN_WIDTH, 2.f));
            wall.setFillColor(sf::Color::Red);

            // Верхняя стена
            wall.setPosition(0.f, 0.f);
            window.draw(wall);

            // Нижняя стена
            wall.setPosition(0.f, SCREEN_HEIGHT - 2.f);
            window.draw(wall);

            // Левая стена
            wall.setSize(sf::Vector2f(2.f, SCREEN_HEIGHT));
            wall.setPosition(0.f, 0.f);
            window.draw(wall);

            // Правая стена
            wall.setPosition(SCREEN_WIDTH - 2.f, 0.f);
            window.draw(wall);
        }

        switch (game.stateGame)
        {
        case MainMenu:
        {
            SetBackgroundColor(game.background, sf::Color::Black);
            DrawUI(game.uiState, game, window);
            break;
        }
        case GameMode:
        {
            DrawUI(game.uiState, game, window);
            break;
        }
        case GameSettingsState:
        {
            DrawUI(game.uiState, game, window);
            break;
        }
        case SoundSettings:
        {
            DrawUI(game.uiState, game, window);
            break;
        }
        case PlayGame:
        {
            SetBackgroundColor(game.background, sf::Color::Green);
            DrawUI(game.uiState, game, window);
            DrawSnake(game.snake, window);
            DrawApple(game.apple, window);
            if (game.settings.stonesEnabled)
            {
                DrawStones(game.stones, window);
            }
            break;
        }
        case ScoreTable:
        {
            DrawUI(game.uiState, game, window);
            break;
        }
        case GameOver:
        {
            SetBackgroundColor(game.background, sf::Color::Black);
            DrawUI(game.uiState, game, window);
            break;
        }
        case EnterName:
        {
            DrawUI(game.uiState, game, window);
            break;
        }
        case ExitConfirm:
        {
            DrawUI(game.uiState, game, window);
            break;
        }
        }
    }
}
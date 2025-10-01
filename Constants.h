#pragma once
#include <string>

namespace SnakeGame
{
    const std::string RESOURCES_PATH = "Resources/";
    const std::string TEXTURE = "/Texture";
    const std::string FONT = "/Font";
    const std::string SOUND = "/Sound";

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    const float SIZE_ITEM = 20.0f;
    const int GRID_WIDTH = SCREEN_WIDTH / static_cast<int>(SIZE_ITEM);
    const int GRID_HEIGHT = SCREEN_HEIGHT / static_cast<int>(SIZE_ITEM);

    const float INITIAL_SPEED = 100.f;
    const int INITIAL_BODY_SEGMENTS = 3;
    const float ACCELERATION = 20.f;
    const float TIMEOUT = 3.0f;
    const std::string PLAYER_NAME = "Player";

    const int MAX_HIGH_SCORES = 10;
    const std::string HIGH_SCORES_FILE = "highscores.dat";
    const std::string SETTINGS_FILE = "settings.dat";
}
#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "character.h"
#include "enemy.h"
#include <vector>

class Game {
public:
    Game();
    ~Game();
    void init();
    void loading_texture();
    void run();
    void logErrorAndExit(const char* msg, const char* error);
    void destroy();
    void quit();
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    int backgroundY = 0;
    bool running;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* characterTexture;
    SDL_Texture* arrowTexture;
    SDL_Texture* background;
    SDL_Texture* enemyIdleTexture; // Texture cho trạng thái idle
    SDL_Texture* enemyAttackTexture; // Texture cho trạng thái tấn công
    SDL_Texture* laserTexture;
    Character character; // Lưu Character
    std::vector<Enemy> enemies;
    SDL_Event event;
    Uint32 lastEnemySpawnTime;
    Mix_Chunk* hitSound;
    int score;
    TTF_Font* font;
    SDL_Texture* scoreTexture;
    SDL_Rect scoreRect;
    void updateScoreTexture();
};

#endif // GAME_H


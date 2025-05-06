#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "character.h"
#include "enemy.h"
#include "boss.h"
#include <vector>

class Game {
public:
    enum GameState { MENU, PLAYING, GAME_OVER };
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
    SDL_Texture* bossAttackTexture;
    SDL_Texture* bossDeathTexture;
    SDL_Texture* bossHitTexture;
    Character character; // Lưu Character
    std::vector<Enemy> enemies;
    Boss* boss;
    bool hasBoss;
    Uint32 lastBossSpawnTime;
    SDL_Event event;
    Uint32 lastEnemySpawnTime;
    Mix_Chunk* hitSound;
    Mix_Chunk* bossAttackSound;
    Mix_Chunk* gameOverSound;
    Mix_Chunk* gameStartSound;
    int score;
    int highScore;
    TTF_Font* font;
    SDL_Texture* scoreTexture;
    SDL_Rect scoreRect;
    SDL_Texture* highScoreTexture;
    SDL_Rect highScoreRect;
    SDL_Texture* healthTexture; // Texture để hiển thị máu
    SDL_Rect healthRect;

    // Menu
    GameState gameState;
    int selectedMenuItem; // 0: Start Game/Play Again, 1: Quit
    SDL_Texture* startGameTexture;
    SDL_Rect startGameRect;
    SDL_Texture* quitTexture;
    SDL_Rect quitRect;
    SDL_Texture* gameOverTextTexture; // Văn bản "Game Over"
    SDL_Rect gameOverTextRect;
    SDL_Texture* playAgainTexture;
    SDL_Rect playAgainRect;

    void updateScoreTexture();
    void updateHighScoreTexture();
    void updateHealthTexture();
    void updateGameOverTexture();
    void initMenu();
    void renderMenu();
    void handleMenuInput();
    void resetGame();
    void loadHighScore();
    void saveHighScore();
};

#endif // GAME_H

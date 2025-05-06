#include "game.h"
#include "character.h"
#include "arrow.h"
#include "enemy.h"
#include "boss.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <fstream>

Game::Game() :  running(false), window(nullptr), renderer(nullptr), characterTexture(nullptr), arrowTexture(nullptr), enemyIdleTexture(nullptr),
                enemyAttackTexture(nullptr), laserTexture(nullptr), bossAttackTexture(nullptr),bossDeathTexture(nullptr), bossHitTexture(nullptr),
                boss(nullptr), hasBoss(false), lastBossSpawnTime(0),
                lastEnemySpawnTime(0), hitSound(nullptr), bossAttackSound(nullptr), gameOverSound(nullptr), gameStartSound(nullptr), score(0), font(nullptr),
                scoreTexture(nullptr), highScoreTexture(nullptr), healthTexture(nullptr),
                gameState(MENU), selectedMenuItem(0), startGameTexture(nullptr), quitTexture(nullptr),
                gameOverTextTexture(nullptr), playAgainTexture(nullptr){
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void Game::logErrorAndExit(const char* msg, const char* error) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
    exit(1);
}

void Game::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logErrorAndExit("SDL_Init", SDL_GetError());

    window = SDL_CreateWindow("Archery Defender", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
        logErrorAndExit("SDL_image error:", IMG_GetError());

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        logErrorAndExit("Mix_OpenAudio", Mix_GetError());
    }
    if (TTF_Init() == -1) {
        logErrorAndExit("TTF_Init", TTF_GetError());
    }
    running = true;
    loading_texture();
    initMenu();
    loadHighScore();
}

SDL_Texture* loadTexture(const char* filePath, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(filePath);
    if (!surface) {
        std::cout << "Error loading image: " << filePath << " - " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::cout << "Error creating texture: " << SDL_GetError() << std::endl;
    }
    return texture;
}

void Game::loading_texture() {
    background = loadTexture("assets/images/background.png", renderer);
    characterTexture = loadTexture("assets/images/character_sprites.png", renderer);
    arrowTexture = loadTexture("assets/images/arrow.png", renderer);
    enemyIdleTexture = loadTexture("assets/images/enemy.png", renderer);
    enemyAttackTexture = loadTexture("assets/images/enemy_attack_sprites.png", renderer);
    laserTexture = loadTexture("assets/images/laser.png", renderer);
    bossAttackTexture = loadTexture("assets/images/boss_attack_sprites.png", renderer);
    bossDeathTexture = loadTexture("assets/images/boss_death_sprites.png", renderer);
    bossHitTexture = loadTexture("assets/images/boss_hit.png", renderer);
    if (!characterTexture || !arrowTexture || !background || !enemyIdleTexture || !enemyAttackTexture || !laserTexture || !bossAttackTexture || !bossDeathTexture || !bossHitTexture ) {
        logErrorAndExit("Failed to load textures", "Check image files");
    }
    // Đặt nhân vật ở giữa phía dưới: x = 368, y = 526
    character = Character(characterTexture, arrowTexture, 368, 526);
    // Tải âm thanh
    hitSound = Mix_LoadWAV("assets/sounds/hit.wav");
    if (!hitSound) {
        logErrorAndExit("Mix_LoadWAV hit.wav", Mix_GetError());
    }
    bossAttackSound = Mix_LoadWAV("assets/sounds/boss_attack.wav");
    if (!bossAttackSound) {
        logErrorAndExit("Mix_LoadWAV boss_attack.wav", Mix_GetError());
    }

    gameOverSound = Mix_LoadWAV("assets/sounds/game_over.wav");
    if (!gameOverSound) {
        logErrorAndExit("Mix_LoadWAV game_over.wav", Mix_GetError());
    }

    gameStartSound = Mix_LoadWAV("assets/sounds/game_start.wav");
    if (!gameStartSound) {
        logErrorAndExit("Mix_LoadWAV game_start.wav", Mix_GetError());
    }

    font = TTF_OpenFont("assets/fonts/arial.ttf", 24); // Cỡ chữ 24
    if (!font) {
        logErrorAndExit("TTF_OpenFont", TTF_GetError());
    }
    // Khởi tạo texture điểm số ban đầu
    score = 0;
    updateScoreTexture();
    updateHighScoreTexture();
    updateHealthTexture();
}

void Game::loadHighScore() {
    std::ifstream inFile("highscore.txt");
    if (inFile.is_open()) {
        inFile >> highScore;
        inFile.close();
    } else {
        highScore = 0; // Nếu không có file, High Score mặc định là 0
        std::cout << "No highscore.txt found, setting High Score to 0" << std::endl;
    }
    updateHighScoreTexture();
}

void Game::saveHighScore() {
    if (score > highScore) {
        highScore = score;
        std::ofstream outFile("highscore.txt");
        if (outFile.is_open()) {
            outFile << highScore;
            outFile.close();
        } else {
            std::cout << "Failed to save High Score to highscore.txt" << std::endl;
        }
        updateHighScoreTexture();
    }
}

void Game::initMenu() {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface;

    // Start Game
    surface = TTF_RenderText_Solid(font, "Start Game", white);
    if (!surface) logErrorAndExit("TTF_RenderText_Solid", TTF_GetError());
    startGameTexture = SDL_CreateTextureFromSurface(renderer, surface);
    startGameRect = { (SCREEN_WIDTH - surface->w) / 2, 300, surface->w, surface->h };
    SDL_FreeSurface(surface);

    // Quit
    surface = TTF_RenderText_Solid(font, "Quit", white);
    if (!surface) logErrorAndExit("TTF_RenderText_Solid", TTF_GetError());
    quitTexture = SDL_CreateTextureFromSurface(renderer, surface);
    quitRect = { (SCREEN_WIDTH - surface->w) / 2, 350, surface->w, surface->h };
    SDL_FreeSurface(surface);

    // Game Over
    surface = TTF_RenderText_Solid(font, "Game Over", white);
    if (!surface) logErrorAndExit("TTF_RenderText_Solid", TTF_GetError());
    gameOverTextTexture = SDL_CreateTextureFromSurface(renderer, surface);
    gameOverTextRect = { (SCREEN_WIDTH - surface->w) / 2, 200, surface->w, surface->h };
    SDL_FreeSurface(surface);

    // Play Again
    surface = TTF_RenderText_Solid(font, "Play Again", white);
    if (!surface) logErrorAndExit("TTF_RenderText_Solid", TTF_GetError());
    playAgainTexture = SDL_CreateTextureFromSurface(renderer, surface);
    playAgainRect = { (SCREEN_WIDTH - surface->w) / 2, 300, surface->w, surface->h };
    SDL_FreeSurface(surface);
}

void Game::renderMenu() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (gameState == GAME_OVER) {
        SDL_RenderCopy(renderer, gameOverTextTexture, nullptr, &gameOverTextRect);
    }

    // Màu của mục được chọn là vàng, mục không được chọn là trắng
    SDL_Color selectedColor = {255, 255, 0, 255}; // Vàng
    SDL_Color unselectedColor = {255, 255, 255, 255}; // Trắng
    SDL_Surface* surface;

    // Render "Start Game" hoặc "Play Again"
    if (selectedMenuItem == 0) {
        surface = TTF_RenderText_Solid(font, gameState == MENU ? "Start Game" : "Play Again", selectedColor);
    } else {
        surface = TTF_RenderText_Solid(font, gameState == MENU ? "Start Game" : "Play Again", unselectedColor);
    }
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect tempRect = { (SCREEN_WIDTH - surface->w) / 2, 300, surface->w, surface->h };
    SDL_RenderCopy(renderer, tempTexture, nullptr, &tempRect);
    SDL_DestroyTexture(tempTexture);
    SDL_FreeSurface(surface);

    // Render "Quit"
    if (selectedMenuItem == 1) {
        surface = TTF_RenderText_Solid(font, "Quit", selectedColor);
    } else {
        surface = TTF_RenderText_Solid(font, "Quit", unselectedColor);
    }
    tempTexture = SDL_CreateTextureFromSurface(renderer, surface);
    tempRect = { (SCREEN_WIDTH - surface->w) / 2, 350, surface->w, surface->h };
    SDL_RenderCopy(renderer, tempTexture, nullptr, &tempRect);
    SDL_DestroyTexture(tempTexture);
    SDL_FreeSurface(surface);

    SDL_RenderPresent(renderer);
}

void Game::handleMenuInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
            return;
        }
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    selectedMenuItem = (selectedMenuItem == 0) ? 1 : 0;
                    break;
                case SDLK_DOWN:
                    selectedMenuItem = (selectedMenuItem == 0) ? 1 : 0;
                    break;
                case SDLK_RETURN:
                    if (selectedMenuItem == 0) { // Start Game hoặc Play Again
                        gameState = PLAYING;
                        if (gameStartSound) {
                            Mix_PlayChannel(-1, gameStartSound, 0);
                        }
                        if (gameState == PLAYING) {
                            resetGame();
                        }
                    } else { // Quit
                        running = false;
                    }
                    break;
            }
        }
    }
}

void Game::resetGame() {
    // Reset trạng thái trò chơi
    character = Character(characterTexture, arrowTexture, 352, 494);
    enemies.clear();
    if (boss) {
        delete boss;
        boss = nullptr;
    }
    hasBoss = false;
    lastBossSpawnTime = SDL_GetTicks(); // Đặt thời gian spawn boss bằng thời gian hiện tại để tránh spawn ngay
    lastEnemySpawnTime = SDL_GetTicks();
    score = 0;
    updateScoreTexture();
    updateHealthTexture();
}

void Game::updateScoreTexture() {
    // Chuyển điểm số thành chuỗi
    std::stringstream ss;
    ss << "Score: " << score;
    std::string scoreText = ss.str();

    // Tạo surface từ văn bản
    SDL_Color textColor = {255, 255, 255, 255}; // Màu trắng
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    if (!textSurface) {
        logErrorAndExit("TTF_RenderText_Solid", TTF_GetError());
    }

    // Tạo texture từ surface
    if (scoreTexture) {
        SDL_DestroyTexture(scoreTexture); // Giải phóng texture cũ
    }
    scoreTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!scoreTexture) {
        logErrorAndExit("SDL_CreateTextureFromSurface", SDL_GetError());
    }

    // Cập nhật vùng vẽ
    scoreRect = { 10, 10, textSurface->w, textSurface->h }; // Góc trên bên trái
    SDL_FreeSurface(textSurface);
}

void Game::updateHighScoreTexture() {
    std::stringstream ss;
    ss << "High Score: " << highScore;
    std::string highScoreText = ss.str();

    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, highScoreText.c_str(), textColor);
    if (!textSurface) {
        logErrorAndExit("TTF_RenderText_Solid", TTF_GetError());
    }

    if (highScoreTexture) {
        SDL_DestroyTexture(highScoreTexture);
    }
    highScoreTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!highScoreTexture) {
        logErrorAndExit("SDL_CreateTextureFromSurface", SDL_GetError());
    }

    highScoreRect = { 10, 40, textSurface->w, textSurface->h }; // Bên dưới Score
    SDL_FreeSurface(textSurface);
}

void Game::updateHealthTexture() {
    std::stringstream ss;
    ss << "HP: " << character.health;
    std::string healthText = ss.str();

    SDL_Color textColor = {255, 0, 0, 255}; // Màu đỏ
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, healthText.c_str(), textColor);
    if (!textSurface) {
        logErrorAndExit("TTF_RenderText_Solid", TTF_GetError());
    }

    if (healthTexture) {
        SDL_DestroyTexture(healthTexture);
    }
    healthTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!healthTexture) {
        logErrorAndExit("SDL_CreateTextureFromSurface", SDL_GetError());
    }

    healthRect = { SCREEN_WIDTH - textSurface->w - 10, 10, textSurface->w, textSurface->h }; // Góc trên bên phải
    SDL_FreeSurface(textSurface);
}


void Game::run() {
    if (!running) return;
    while (running) {
        if (gameState == MENU || gameState == GAME_OVER) {
            handleMenuInput();
            renderMenu();
        } else if (gameState == PLAYING) {
           while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            character.handleInput(event);
        }

        backgroundY += 2;
        if (backgroundY >= SCREEN_HEIGHT) {
            backgroundY = 0;
        }

        int currentTime = SDL_GetTicks();
        character.updateAnimation(currentTime);
        character.update(SCREEN_WIDTH, SCREEN_HEIGHT);

        if (character.isAlive && currentTime - lastEnemySpawnTime >= 2000) { // 2000ms = 2 giây
            // Tọa độ ngẫu nhiên ở phía trên (y = -100 để xuất hiện ngoài màn hình)
            float spawnX = static_cast<float>(std::rand() % (SCREEN_WIDTH - 100)); // 100 là frameWidth của kẻ địch
            enemies.emplace_back(enemyIdleTexture, enemyAttackTexture, laserTexture, spawnX, -100);
            lastEnemySpawnTime = currentTime;
        }
        // Xuất hiện boss sau mỗi 40 giây
        if (character.isAlive && !hasBoss && currentTime - lastBossSpawnTime >= 40000) {
            float spawnX = (SCREEN_WIDTH - 162) / 2; // Ở giữa phía trên
            float spawnY = 0;
            boss = new Boss(bossAttackTexture, bossDeathTexture, bossHitTexture, spawnX, spawnY);
            hasBoss = true;
            lastBossSpawnTime = currentTime;
        }
        // Cập nhật kẻ địch
        for (auto& enemy : enemies) {
            enemy.updateAnimation(currentTime);
            float playerX = character.x + character.frameWidth / 2;
            float playerY = character.y + character.frameHeight / 2;
            enemy.update(playerX, playerY);
        }
        if (hasBoss && boss) {
            boss->updateAnimation(currentTime);
            float playerX = character.x + character.frameWidth / 2;
            float playerY = character.y + character.frameHeight / 2;
            boss->update(playerX, playerY, bossAttackSound);
            if (!boss->isAlive && !boss->isDying) {
                delete boss;
                boss = nullptr;
                hasBoss = false;
                score += 150; // Thưởng điểm khi hạ boss
                updateScoreTexture();
                saveHighScore();
            }
        }
        // Kiểm tra va chạm giữa mũi tên và kẻ địch
        auto& arrows = character.arrows;
        for (auto arrowIt = arrows.begin(); arrowIt != arrows.end();) {
            bool arrowRemoved = false;

            for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();) {
                if (SDL_HasIntersection(&arrowIt->collisionRect, &enemyIt->collisionRect)) {
                    Mix_PlayChannel(-1, hitSound, 0);
                    score += 10; // Tăng điểm khi bắn hạ
                    updateScoreTexture(); // Cập nhật texture điểm số
                    saveHighScore();
                    enemyIt = enemies.erase(enemyIt);
                    arrowIt = arrows.erase(arrowIt);
                    arrowRemoved = true;
                    break;
                } else {
                    ++enemyIt;
                }
            }
            // Va chạm mũi tên-boss
            if (!arrowRemoved && hasBoss && boss && boss->isAlive) {
                if (SDL_HasIntersection(&arrowIt->collisionRect, &boss->collisionRect)) {
                    boss->health--;
                    Mix_PlayChannel(-1, hitSound, 0);
                    if (boss->health <= 0) {
                        boss->isAlive = false;
                        boss->isDying = true;
                        boss->currentFrame = 0; // Reset animation chết
                    }
                    arrowIt = arrows.erase(arrowIt);
                    arrowRemoved = true;
                }
            }
            if (!arrowRemoved) {
                ++arrowIt;
            }
        }
        // Kiểm tra va chạm laser-nhân vật
        if (character.isAlive) {
            for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ++enemyIt) {
                auto& lasers = enemyIt->lasers;
                for (auto laserIt = lasers.begin(); laserIt != lasers.end();) {
                    if (SDL_HasIntersection(&laserIt->destRect, &character.collisionRect)) {
                        character.health -= 10; // Giảm 10 HP mỗi lần trúng
                        updateHealthTexture();
                        laserIt = lasers.erase(laserIt);
                        if (character.health <= 0) {
                            character.isAlive = false;
                            gameState = GAME_OVER;
                            selectedMenuItem = 0;
                            if (gameOverSound) {
                                Mix_PlayChannel(-1, gameOverSound, 0); // Phát âm thanh game over
                            }
                            saveHighScore();
                        }
                    } else {
                        ++laserIt;
                    }
                }
            }
        }
        // Va chạm boss hit-nhân vật
            if (hasBoss && boss && boss->isAlive) {
                auto& hits = boss->hits;
                for (auto hitIt = hits.begin(); hitIt != hits.end();) {
                    if (SDL_HasIntersection(&hitIt->collisionRect, &character.collisionRect)) {
                        character.health -= 20; // Sát thương từ boss
                        updateHealthTexture();
                        hitIt = hits.erase(hitIt);
                        if (character.health <= 0) {
                            character.isAlive = false;
                            gameState = GAME_OVER;
                            selectedMenuItem = 0;
                            if (gameOverSound) {
                                Mix_PlayChannel(-1, gameOverSound, 0); // Phát âm thanh game over
                            }
                            saveHighScore();
                        }
                    } else {
                        ++hitIt;
                    }
                }
            }
        // Xóa kẻ địch ra khỏi màn hình
        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [this](const Enemy& enemy) { return enemy.isOffScreen(SCREEN_HEIGHT); }),
            enemies.end()
        );
        // Vẽ background
        SDL_Rect backgroundrect = {0, backgroundY, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, background, nullptr, &backgroundrect);
        backgroundrect.y = backgroundY - SCREEN_HEIGHT;
        SDL_RenderCopy(renderer, background, nullptr, &backgroundrect);
        character.render(renderer);

        if (character.isAlive) {
                character.render(renderer);
                for (const auto& enemy : enemies) {
                    enemy.render(renderer);
                }
                if (hasBoss && boss) {
                    boss->render(renderer);
                }
                if (scoreTexture) {
                    SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);
                }
                if (highScoreTexture) {
                    SDL_RenderCopy(renderer, highScoreTexture, nullptr, &highScoreRect);
                }
                if (healthTexture) {
                    SDL_RenderCopy(renderer, healthTexture, nullptr, &healthRect);
                }
            }
        SDL_RenderPresent(renderer);
        }
    }
}
void Game::destroy() {
    if (boss) {
        delete boss;
        boss = nullptr;
    }
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(characterTexture);
    SDL_DestroyTexture(arrowTexture);
    SDL_DestroyTexture(enemyIdleTexture);
    SDL_DestroyTexture(enemyAttackTexture);
    SDL_DestroyTexture(laserTexture);
    SDL_DestroyTexture(bossAttackTexture);
    SDL_DestroyTexture(bossDeathTexture);
    SDL_DestroyTexture(bossHitTexture);
    SDL_DestroyTexture(startGameTexture);
    SDL_DestroyTexture(quitTexture);
    SDL_DestroyTexture(gameOverTextTexture);
    SDL_DestroyTexture(playAgainTexture);
    SDL_DestroyTexture(highScoreTexture);
    Mix_FreeChunk(hitSound);
    Mix_FreeChunk(bossAttackSound);
    Mix_FreeChunk(gameOverSound);
    Mix_FreeChunk(gameStartSound);
    Mix_CloseAudio();
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(healthTexture);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::quit() {
    running = false;
}

Game::~Game() {
    destroy();
}

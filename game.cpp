#include "game.h"
#include "character.h"
#include "arrow.h"
#include "enemy.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Game::Game() : running(false), window(nullptr), renderer(nullptr), characterTexture(nullptr), arrowTexture(nullptr), enemyTexture(nullptr), lastEnemySpawnTime(0), hitSound(nullptr){// Khởi tạo seed cho số ngẫu nhiên
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
    running = true;
    loading_texture();
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
    enemyTexture = loadTexture("assets/images/enemy.png", renderer);
    if (!characterTexture || !arrowTexture || !background || !enemyTexture) {
        logErrorAndExit("Failed to load textures", "Check image files");
    }
    // Đặt nhân vật ở giữa phía dưới: x = 368, y = 526
    character = Character(characterTexture, arrowTexture, 368, 526);
    // Tải âm thanh
    hitSound = Mix_LoadWAV("assets/sounds/hit.wav");
    if (!hitSound) {
        logErrorAndExit("Mix_LoadWAV hit.wav", Mix_GetError());
    }
}

void Game::run() {
    if (!running) return;
    while (running) {
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
        // Tạo kẻ địch mỗi 1 giây
        if (currentTime - lastEnemySpawnTime >= 1000) { // 1000ms = 1 giây
            // Tọa độ ngẫu nhiên ở phía trên (y = -100 để xuất hiện ngoài màn hình)
            float spawnX = static_cast<float>(std::rand() % (SCREEN_WIDTH - 100)); // 100 là frameWidth của kẻ địch
            enemies.emplace_back(enemyTexture, spawnX, -100);
            lastEnemySpawnTime = currentTime;
        }
        // Cập nhật kẻ địch
        for (auto& enemy : enemies) {
            enemy.updateAnimation(currentTime);
            enemy.update();
        }
        // Kiểm tra va chạm giữa mũi tên và kẻ địch
        auto& arrows = character.arrows;
        for (auto arrowIt = arrows.begin(); arrowIt != arrows.end();) {
            bool arrowRemoved = false;
            for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();) {
                if (SDL_HasIntersection(&arrowIt->collisionRect, &enemyIt->collisionRect)) {
                    Mix_PlayChannel(-1, hitSound, 0);
                    enemyIt = enemies.erase(enemyIt);
                    arrowIt = arrows.erase(arrowIt);
                    arrowRemoved = true;
                    break;
                } else {
                    ++enemyIt;
                }
            }
            if (!arrowRemoved) {
                ++arrowIt;
            }
        }
        // Xóa kẻ địch ra khỏi màn hình
        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [this](const Enemy& enemy) { return enemy.isOffScreen(SCREEN_HEIGHT); }),
            enemies.end()
        );
        // Vẽ
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect backgroundrect = {0, backgroundY, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, background, nullptr, &backgroundrect);
        backgroundrect.y = backgroundY - SCREEN_HEIGHT;
        SDL_RenderCopy(renderer, background, nullptr, &backgroundrect);
        character.render(renderer);
        for (const auto& enemy : enemies) {
            enemy.render(renderer);
        }
        SDL_RenderPresent(renderer);
    }
}
void Game::destroy() {
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(characterTexture);
    SDL_DestroyTexture(arrowTexture);
    SDL_DestroyTexture(enemyTexture);
    Mix_FreeChunk(hitSound);
    Mix_CloseAudio();
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

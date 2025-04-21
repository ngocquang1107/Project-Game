#include "character.h"
#include <iostream>

Character::Character() : texture(nullptr), arrowTexture(nullptr), x(0), y(0), speed(0), angle(0) , lastShootTime(0) {
    srcRect = {0, 0, 0, 0};
    destRect = {0, 0, 0, 0};
    frameWidth = 0;
    frameHeight = 0;
    currentFrame = 0;
    totalFrames = 0;
    frameTime = 0;
    lastFrameTime = 0;
    moveLeft = moveRight = false;
}

Character::Character(SDL_Texture* charTex, SDL_Texture* arrTex, int x, int y) {
    texture = charTex;
    arrowTexture = arrTex;
    frameWidth = 96;
    frameHeight = 96;
    totalFrames = 4;
    currentFrame = 0;
    frameTime = 100;
    lastFrameTime = 0;
    srcRect = { 0, 0, frameWidth, frameHeight };
    destRect = { x, y, frameWidth, frameHeight };
    this->x = x;
    this->y = y;
    speed = 4.0f;
    angle = -90.0; // Xoay 90 độ sang trái
    moveLeft = moveRight = false;
    lastShootTime = 0; // Khởi tạo thời gian bắn
}

void Character::handleInput(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
        switch (event.key.keysym.sym) {
            case SDLK_a:
            case SDLK_LEFT:
                moveLeft = true;
                break;
            case SDLK_d:
            case SDLK_RIGHT:
                moveRight = true;
                break;
        }
    }
    else if (event.type == SDL_KEYUP && event.key.repeat == 0) {
        switch (event.key.keysym.sym) {
            case SDLK_a:
            case SDLK_LEFT:
                moveLeft = false;
                break;
            case SDLK_d:
            case SDLK_RIGHT:
                moveRight = false;
                break;
        }
    }
}

void Character::updateAnimation(int currentTime) {
    if (currentTime - lastFrameTime > frameTime) {
        currentFrame = (currentFrame + 1) % totalFrames;
        srcRect.x = currentFrame * frameWidth;
        lastFrameTime = currentTime;
    }
}

void Character::update(int screenWidth, int screenHeight) {
    // Chỉ di chuyển trái/phải
    if (moveLeft) x -= speed;
    if (moveRight) x += speed;

    // Giới hạn di chuyển trong màn hình (chỉ theo chiều ngang)
    if (x < 0) x = 0;
    if (x + frameWidth > screenWidth) x = screenWidth - frameWidth;

    destRect.x = (int)x;
    destRect.y = (int)y;

    // Bắn tự động mỗi 0,5 giây
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastShootTime >= 500) { // 500ms = 0,5 giây
        arrows.emplace_back(arrowTexture, x + frameWidth / 2 - 22, y - 22); // Vị trí cung
        lastShootTime = currentTime;
    }

    for (auto& arrow : arrows) {
        arrow.update();
    }

    arrows.erase(
        std::remove_if(arrows.begin(), arrows.end(),
            [screenHeight](const Arrow& arrow) { return arrow.isOffScreen(screenHeight); }),
        arrows.end()
    );
}

void Character::render(SDL_Renderer* renderer) {
    if (!texture) std::cout << "Character texture is null" << std::endl;
    SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, angle, nullptr, SDL_FLIP_NONE);
    for (auto& arrow : arrows) {
        arrow.render(renderer);
    }
}

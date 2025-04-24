#include "laser.h"
#include <cmath>

Laser::Laser(SDL_Texture* tex, float startX, float startY, float dirX, float dirY) {
    texture = tex;
    x = startX;
    y = startY;
    speed = 2.0f;
    frameWidth = 40;
    frameHeight = 60;
    totalFrames = 7; // 7 frame cho laser
    currentFrame = 0;
    frameTime = 100;
    lastFrameTime = 0;
    srcRect = { 0, 0, frameWidth, frameHeight };
    destRect = { (int)x, (int)y, frameWidth, frameHeight };

    float length = std::sqrt(dirX * dirX + dirY * dirY);
    if (length != 0) {
        directionX = dirX / length;
        directionY = dirY / length;
    } else {
        directionX = 0;
        directionY = 0;
    }
}

void Laser::updateAnimation(int currentTime) {
    if (currentTime - lastFrameTime > frameTime) {
        currentFrame = (currentFrame + 1) % totalFrames;
        srcRect.x = currentFrame * frameWidth;
        lastFrameTime = currentTime;
    }
}

void Laser::update() {
    x += directionX * speed;
    y += directionY * speed;
    destRect.x = (int)x;
    destRect.y = (int)y;

    updateAnimation(SDL_GetTicks());
}

void Laser::render(SDL_Renderer* renderer) const {
    double angle = std::atan2(directionY, directionX) * 180 / M_PI;
    SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, angle, nullptr, SDL_FLIP_NONE);
}

bool Laser::isOffScreen(int screenWidth, int screenHeight) const {
    return x < 0 || x > screenWidth || y < 0 || y > screenHeight;
}

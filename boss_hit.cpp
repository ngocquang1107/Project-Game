#include "boss_hit.h"

BossHit::BossHit(SDL_Texture* tex, float startX, float startY) {
    texture = tex;
    x = startX;
    y = startY;
    speed = 3.0f;
    srcRect = { 0, 0, 32, 48 };
    destRect = { (int)x, (int)y, 32, 48 };
    collisionRect = { (int)x, (int)y, 32, 48 };
}

void BossHit::update() {
    y += speed;
    destRect.y = (int)y;
    collisionRect.y = (int)y;
}

void BossHit::render(SDL_Renderer* renderer) const {
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

bool BossHit::isOffScreen(int screenHeight) const {
    return y > screenHeight;
}

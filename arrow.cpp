#include "arrow.h"

Arrow::Arrow(SDL_Texture* tex, float startX, float startY) {
    texture = tex;
    x = startX;
    y = startY;
    speed = 4.0f;
    destRect = { (int)x, (int)y, 24, 12 }; // Kích thước mũi tên
    collisionRect = { (int)x, (int)y, 8, 12 };
}

void Arrow::update() {
    y -= speed; // Di chuyển lên trên
    destRect.y = (int)y;
    collisionRect.y = (int)y;
    collisionRect.x = (int)x;
}

void Arrow::render(SDL_Renderer* renderer) const {
    SDL_RenderCopyEx(renderer, texture, nullptr, &destRect, -90.0, nullptr, SDL_FLIP_NONE);
}

bool Arrow::isOffScreen(int screenHeight) const {
    return y < 0; // Mũi tên ra khỏi mép trên
}

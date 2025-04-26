#include "boss.h"
#include <iostream>
#include <algorithm>

Boss::Boss(SDL_Texture* attackTex, SDL_Texture* deathTex, SDL_Texture* hitTex, float startX, float startY) {
    attackTexture = attackTex;
    deathTexture = deathTex;
    hitTexture = hitTex;
    x = startX;
    y = startY;
    frameWidth = 162; // Kích thước frame khi sống
    frameHeight = 142;
    currentFrame = 0;
    totalFrames = 8; // 8 frame cho animation tấn công
    frameTime = 100;
    lastFrameTime = 0;
    srcRect = { 0, 0, frameWidth, frameHeight };
    destRect = { (int)x, (int)y, frameWidth, frameHeight };
    collisionRect = { (int)x + 10, (int)y + 10, frameWidth - 20, frameHeight - 20 }; // Vùng va chạm nhỏ hơn sprite
    lastAttackTime = 0;
    health = 15; // Cần 15 mũi tên để hạ
    isAlive = true;
    isDying = false;
}

void Boss::updateAnimation(int currentTime) {
    if (isDying) {
        frameWidth = 189; // Kích thước frame khi chết
        frameHeight = 142;
        totalFrames = 6; // 6 frame cho animation chết
    } else {
        frameWidth = 162; // Kích thước frame khi sống
        frameHeight = 142;
        totalFrames = 8; // 8 frame cho animation tấn công
    }

    if (currentTime - lastFrameTime > frameTime) {
        currentFrame = (currentFrame + 1) % totalFrames;
        srcRect = { currentFrame * frameWidth, 0, frameWidth, frameHeight };
        lastFrameTime = currentTime;
    }
}

void Boss::update(float playerX, float playerY) {
    if (!isAlive && !isDying) return; // Đã chết hoàn toàn, không cập nhật

    if (isAlive) {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastAttackTime >= 5000) { // Tấn công mỗi 5 giây
            float hitStartX = x + (frameWidth - 32) / 2; // Căn giữa boss
            float hitStartY = y + frameHeight;
            hits.emplace_back(hitTexture, hitStartX, hitStartY);
            lastAttackTime = currentTime;
        }

        for (auto& hit : hits) {
            hit.update();
        }

        hits.erase(
            std::remove_if(hits.begin(), hits.end(),
                [this](const BossHit& hit) { return hit.isOffScreen(600); }),
            hits.end()
        );
    } else if (isDying) {
        // Animation chết chạy hết thì dừng
        if (currentFrame == totalFrames - 1) {
            isDying = false; // Đã chạy xong animation chết
        }
    }

    destRect = { (int)x, (int)y, frameWidth, frameHeight };
    collisionRect = { (int)x + 10, (int)y + 10, frameWidth - 20, frameHeight - 20 };
}

void Boss::render(SDL_Renderer* renderer) const {
    if (!isAlive && !isDying) return; // Không render nếu đã chết hoàn toàn

    SDL_Texture* currentTexture = isDying ? deathTexture : attackTexture;
    SDL_RenderCopy(renderer, currentTexture, &srcRect, &destRect);

    for (const auto& hit : hits) {
        hit.render(renderer);
    }
}

#include "enemy.h"
#include <cmath>
#include <algorithm>

Enemy::Enemy(SDL_Texture* idleTex, SDL_Texture* attackTex, SDL_Texture* laserTex, float startX, float startY) {
    idleTexture = idleTex;
    attackTexture = attackTex;
    laserTexture = laserTex;
    x = startX;
    y = startY;
    speed = 1.0f;
    frameWidth = 100;  // Kích thước mỗi frame
    frameHeight = 100;
    idleFrames = 4;
    attackFrames = 7;
    totalFrames = idleFrames; // Mặc định là idle
    currentFrame = 0;
    frameTime = 100;   // Thời gian mỗi frame (ms)
    lastFrameTime = 0;
    srcRect = { 0, 0, frameWidth, frameHeight };
    destRect = { (int)x, (int)y, frameWidth, frameHeight };
    // collisionRect: 80x80, căn giữa destRect
    int collWidth = 80;  // 80% chiều rộng
    int collHeight = 80; // 80% chiều cao
    collisionRect = { (int)x + (frameWidth - collWidth) / 2, (int)y + (frameHeight - collHeight) / 2, collWidth, collHeight };
    isAttacking = false;
    lastLaserTime = 0;
}

void Enemy::updateAnimation(int currentTime) {
    if (currentTime - lastFrameTime > frameTime) {
        currentFrame = (currentFrame + 1) % totalFrames;
        srcRect.x = currentFrame * frameWidth;
        lastFrameTime = currentTime;
    }
}

void Enemy::update(float playerX, float playerY) {
    y += speed; // Di chuyển xuống dưới
    destRect.y = (int)y;
    collisionRect.y = (int)y + (frameHeight - collisionRect.h) / 2; // Cập nhật collisionRect
    collisionRect.x = (int)x + (frameWidth - collisionRect.w) / 2;

    // Kiểm tra khoảng cách với nhân vật
    float dx = playerX - (x + frameWidth / 2);
    float dy = playerY - (y + frameHeight / 2);
    float distance = std::sqrt(dx * dx + dy * dy);

    // Chuyển trạng thái và bắn laser
    isAttacking = (distance < 700);
    totalFrames = isAttacking ? attackFrames : idleFrames; // Chuyển đổi số frame
    frameTime = isAttacking ? 50 : 100; // 50ms khi tấn công, 100ms khi idle

    Uint32 currentTime = SDL_GetTicks();
    if (distance < 700 && currentTime - lastLaserTime >= 1000) {
        float startX = x + frameWidth / 2 - 20; // 20 là nửa chiều rộng laser (40/2)
        float startY = y + frameHeight / 2 - 30; // 30 là nửa chiều cao laser (60/2)
        lasers.emplace_back(laserTexture, startX, startY, dx, dy);
        lastLaserTime = currentTime;
    }

    for (auto& laser : lasers) {
        laser.update();
    }

    lasers.erase(
        std::remove_if(lasers.begin(), lasers.end(),
            [this](const Laser& laser) { return laser.isOffScreen(800, 600); }),
        lasers.end()
    );
}

void Enemy::render(SDL_Renderer* renderer) const {
    if (isAttacking) {
        SDL_RenderCopy(renderer, attackTexture, &srcRect, &destRect);
    } else {
        SDL_RenderCopy(renderer, idleTexture, &srcRect, &destRect);
    }
    for (const auto& laser : lasers) {
        laser.render(renderer);
    }
}

bool Enemy::isOffScreen(int screenHeight) const {
    return y > screenHeight; // Ra khỏi mép dưới
}

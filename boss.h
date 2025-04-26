#ifndef BOSS_H
#define BOSS_H

#include <SDL.h>
#include <vector>
#include "boss_hit.h"

class Boss {
public:
    Boss(SDL_Texture* attackTex, SDL_Texture* deathTex, SDL_Texture* hitTex, float startX, float startY);
    SDL_Texture* attackTexture;
    SDL_Texture* deathTexture;
    SDL_Texture* hitTexture;
    SDL_Rect srcRect, destRect;
    SDL_Rect collisionRect;
    float x, y;
    int frameWidth, frameHeight;
    int currentFrame, totalFrames;
    int frameTime, lastFrameTime;
    std::vector<BossHit> hits;
    Uint32 lastAttackTime;
    int health; // Số mũi tên cần để hạ boss
    bool isAlive;
    bool isDying; // Trạng thái đang chạy animation chết
    void updateAnimation(int currentTime);
    void update(float playerX, float playerY);
    void render(SDL_Renderer* renderer) const;
};

#endif // BOSS_H

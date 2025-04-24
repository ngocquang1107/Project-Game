#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <vector>
#include "laser.h"

class Enemy {
public:
    Enemy(SDL_Texture* idleTex, SDL_Texture* attackTex, SDL_Texture* laserTex, float startX, float startY);
    SDL_Texture* idleTexture; // Texture cho trạng thái idle
    SDL_Texture* attackTexture; // Texture cho trạng thái tấn công
    SDL_Texture* laserTexture;
    SDL_Rect srcRect, destRect;
    SDL_Rect collisionRect;
    float x, y;
    float speed;
    int frameWidth, frameHeight;
    int currentFrame, totalFrames;
    int idleFrames, attackFrames;
    int frameTime, lastFrameTime;
    bool isAttacking;
    std::vector<Laser> lasers;
    Uint32 lastLaserTime;
    void updateAnimation(int currentTime);
    void update(float playerX, float playerY);
    void render(SDL_Renderer* renderer) const;
    bool isOffScreen(int screenHeight) const;
};

#endif // ENEMY_H

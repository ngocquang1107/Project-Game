#ifndef BOSSHIT_H
#define BOSSHIT_H

#include <SDL.h>

class BossHit {
public:
    BossHit(SDL_Texture* tex, float startX, float startY);
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    SDL_Rect collisionRect;
    float x, y;
    float speed;
    void update();
    void render(SDL_Renderer* renderer) const;
    bool isOffScreen(int screenHeight) const;
};

#endif // BOSSHIT_H

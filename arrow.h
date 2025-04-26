#ifndef ARROW_H
#define ARROW_H

#include <SDL.h>
#include <algorithm>

class Arrow {
public:
    Arrow(SDL_Texture* tex, float startX, float startY);
    SDL_Texture* texture;
    SDL_Rect destRect;
    SDL_Rect collisionRect;
    float x, y;
    float speed;
    void update();
    void render(SDL_Renderer* renderer) const;
    bool isOffScreen(int screenHeight) const; // Đổi screenWidth thành screenHeight
};

#endif // ARROW_H

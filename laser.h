#ifndef LASER_H
#define LASER_H

#include <SDL.h>

class Laser {
public:
    Laser(SDL_Texture* tex, float startX, float startY, float dirX, float dirY);
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    float x, y;
    float speed;
    float directionX, directionY;
    int frameWidth, frameHeight;
    int currentFrame, totalFrames;
    int frameTime, lastFrameTime;
    void update();
    void updateAnimation(int currentTime);
    void render(SDL_Renderer* renderer) const;
    bool isOffScreen(int screenWidth, int screenHeight) const;
};

#endif // LASER_H

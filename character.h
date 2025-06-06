#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL.h>
#include <vector>
#include "arrow.h"

class Character {
public:
    Character();
    Character(SDL_Texture* charTex, SDL_Texture* arrTex, int x, int y);
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    SDL_Rect collisionRect; // Vùng va chạm
    int frameWidth, frameHeight;
    int currentFrame, totalFrames;
    int frameTime, lastFrameTime;
    int health; // Máu của nhân vật
    bool isAlive; // Trạng thái sống
    std::vector<Arrow> arrows;
    SDL_Texture* arrowTexture;
    float x, y;
    float speed;
    double angle;
    bool moveLeft, moveRight;
    Uint32 lastShootTime; // Theo dõi thời gian bắn cuối cùng
    void handleInput(SDL_Event& event);
    void updateAnimation(int currentTime);
    void update(int screenWidth, int screenHeight);
    void render(SDL_Renderer* renderer);
};

#endif // CHARACTER_H

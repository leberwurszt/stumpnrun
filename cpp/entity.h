#pragma once

// SDL libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <cstdint>
#include <cmath>

#include "level.h"

#define PI 3.141592654
#define TICKS_SINCE_LAST_FRAME (*ticksCurrentFrame - *ticksLastFrame)
#define SECONDS_SINCE_LAST_FRAME (TICKS_SINCE_LAST_FRAME * .001)

class Entity
{
    protected:

        SDL_Point pos = { 0, 0 };

        double moveSpeed = 0;
        double fallAcceleration = 0;

        bool falling = false;
        double currentSpeedX = 0;
        double currentSpeedY = 0;

        uint8_t sprite = 0;

        uint64_t ticksMove = 0;
        uint64_t ticksMoveNext = 0;

        uint64_t* ticksLastFrame = NULL;
        uint64_t* ticksCurrentFrame = NULL;

        Level* level = NULL;

        SDL_Rect hitbox = { TILE_SIZE_X, TILE_SIZE_Y, 0, 0 };


    public:

        Entity(Level* level, uint64_t* ticksLastFrame, uint64_t* ticksCurrentFrame);
        ~Entity();

        virtual void Do();
        virtual void Die();
        void Move(SDL_Point pos);
        void PerformMovement();
        bool CheckCollision(SDL_Point pointCheck);

        int GetPosX();
        int GetPosY();
        double GetAngleMove();
        double GetCurrentSpeedX();
        double GetCurrentSpeedY();
        uint8_t GetSprite();

        void SetPos(SDL_Point pos);
        void SetSprite(uint8_t value);
        void SetCurrentSpeedX(double currentSpeedX);
        void SetCurrentSpeedY(double currentSpeedX);

    
};
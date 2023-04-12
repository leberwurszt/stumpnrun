#pragma once
#include "entity.h"

enum
{
    PLAYER_DO_NOTHING,
    PLAYER_MOVE_RIGHT,
    PLAYER_MOVE_LEFT
};

enum
{
    PLAYER_SPRITE_NONE,
    PLAYER_SPRITE_STAND_RIGHT,
    PLAYER_SPRITE_STAND_LEFT
};

class Player : public Entity
{
    protected:
        uint8_t playerMovement = PLAYER_DO_NOTHING;

    public:
        Player(Level* level, uint64_t* ticksLastFrame, uint64_t* ticksCurrentFrame);
        void Do() override;
        void Jump();
        void SetMovementLeft();
        void SetMovementRight();
        void StopMovement();
};
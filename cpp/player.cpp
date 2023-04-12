#include "player.h"

Player::Player(Level* level, uint64_t* ticksLastFrame, uint64_t* ticksCurrentFrame) : Entity(level, ticksLastFrame, ticksCurrentFrame)
{
    this->fallAcceleration = 9.81;
    this->moveSpeed = 5;
    hitbox = { 43, 10, 41, 108 };
    sprite = PLAYER_SPRITE_STAND_RIGHT;
}

void Player::Do()
{
    Entity::Do();
    switch(playerMovement)
    {
        case PLAYER_DO_NOTHING:
            currentSpeedX = 0;
            break;
        
        case PLAYER_MOVE_LEFT:
            currentSpeedX = -moveSpeed;
            sprite = PLAYER_SPRITE_STAND_LEFT;
            break;

        case PLAYER_MOVE_RIGHT:
            currentSpeedX = moveSpeed;
            sprite = PLAYER_SPRITE_STAND_RIGHT;
            break;
    }


}

void Player::Jump()
{
    if(currentSpeedY == 0)
        currentSpeedY = -10;
    return;
}

void Player::SetMovementLeft()
{
    playerMovement = PLAYER_MOVE_LEFT;
}

void Player::SetMovementRight()
{
    playerMovement = PLAYER_MOVE_RIGHT;
}

void Player::StopMovement()
{
    playerMovement = PLAYER_DO_NOTHING;
}
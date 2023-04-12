#include "entity.h"

Entity::Entity(Level* level, uint64_t* ticksLastFrame, uint64_t* ticksCurrentFrame)
{
    this->level = level;
    this->ticksLastFrame = ticksLastFrame;
    this->ticksCurrentFrame = ticksCurrentFrame;
    return;
}

Entity::~Entity()
{
    return;
}
        
void Entity::Do()
{
    // falling
    if(!CheckCollision({ pos.x, pos.y + 1 }))
        currentSpeedY += fallAcceleration * SECONDS_SINCE_LAST_FRAME;

    PerformMovement();

    return;
}

// move Entity depending on current speed and time elapsed since last frame. Simple collision detection (requires improvement)
void Entity::PerformMovement()
{
    bool collisionX = false;
    bool collisionY = false;

    SDL_Point posNew = { pos.x + (int) (currentSpeedX * TICKS_SINCE_LAST_FRAME * TILE_SIZE_X * .001), pos.y + (int) (currentSpeedY * TICKS_SINCE_LAST_FRAME * TILE_SIZE_Y * .001) };

    while(pos.y != posNew.y && !collisionY)
    {
        if(pos.y > posNew.y)
            if(!CheckCollision({ pos.x, pos.y - 1 }))
                --pos.y;
            else
                collisionY = true;
        else
            if(!CheckCollision({ pos.x, pos.y + 1 }))
                ++pos.y;
            else
                collisionY = true;
    }

    if(collisionY)
        currentSpeedY = 0;

    while(pos.x != posNew.x && !collisionX)
    {
        if(pos.x > posNew.x)
            if(!CheckCollision({ pos.x - 1, pos.y }))
                --pos.x;
            else if(!CheckCollision({ pos.x - 1, pos.y - 1}))
            {
                --pos.x;
                --pos.y;
            }
            else
                collisionX = true;
        else
            if(!CheckCollision({ pos.x + 1, pos.y }))
                ++pos.x;
            else if(!CheckCollision({ pos.x + 1, pos.y - 1}))
            {
                ++pos.x;
                --pos.y;
            }
            else
                collisionX = true;
    }


    if(collisionX)
        currentSpeedX = 0;

    return;
}

bool Entity::CheckCollision(SDL_Point pointCheck)
{
    for(int i = pointCheck.x + hitbox.x; i <= pointCheck.x + hitbox.x + hitbox.w; i++)
        if(level->GetMaskStateFromCoord(i, pointCheck.y + hitbox.y) || level->GetMaskStateFromCoord(i, pointCheck.y + hitbox.y + hitbox.h))
            return true;
        
    for(int i = pointCheck.y + hitbox.y; i <= pointCheck.y + hitbox.y + hitbox.h; i++)
        if(level->GetMaskStateFromCoord(pointCheck.x + hitbox.x, i) || level->GetMaskStateFromCoord(pointCheck.x + hitbox.x + hitbox.w, i))
            return true;

    return false;
}


void Entity::Die()
{}


void Entity::Move(SDL_Point pos)
{
    this->pos.x += pos.x;
    this->pos.y += pos.y;
}

int Entity::GetPosX()
{
    return pos.x;
}

int Entity::GetPosY()
{
    return pos.y;
}

double Entity::GetCurrentSpeedX()
{
    return currentSpeedX;
}

double Entity::GetCurrentSpeedY()
{
    return currentSpeedY;
}

uint8_t Entity::GetSprite()
{
    return sprite;
}

void Entity::SetPos(SDL_Point pos)
{
    this->pos = pos;
}

void Entity::SetSprite(uint8_t value)
{
    this->sprite = sprite;
}

void Entity::SetCurrentSpeedX(double currentSpeedX)
{
    this->currentSpeedX = currentSpeedX;
}

void Entity::SetCurrentSpeedY(double currentSpeedY)
{
    this->currentSpeedY = currentSpeedY;
}
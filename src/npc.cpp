#include "../include/npc.hpp"
#include "../include/collision_detector.hpp"
#include <iostream>
#include <raylib.h>
#include <raymath.h>

Npc::Npc(Type type, AssetManager& assets)
    : Character(type, assets)
{
    body.speed = rand() % 3 + 1;
    destroy    = false;
}

std::function<void(const Rectangle&)>
Npc::onObstacleCollision(const Vector2& lastPosition)
{
    return [this, lastPosition](const Rectangle& obstacle)
    {
        auto step = Vector2Subtract(position, lastPosition);
        position  = lastPosition;

        // try moving in x coord
        position = {position.x + step.x, position.y};
        if (CheckCollisionRecs(obstacle, getBoundingBox()))
        {
            position = {position.x - step.x, position.y};
        }

        // try moving in y coord
        position = {position.x, position.y + step.y};
        if (CheckCollisionRecs(obstacle, getBoundingBox()))
        {
            position = {position.x, position.y - step.y};
        }
    };
}

std::function<void(Character&)>
Npc::onCharacterCollision(const Vector2& lastPosition)
{
    return [this, lastPosition](Character& other)
    {
        if (other.type == Type::PLAYER)
        {
            position = lastPosition;

            // if (attackTimer.start().update().hasElapsed())
            // {
            //     other.actualStats.hp -= rand() % actualStats.atk;
            //     attackTimer.reset();
            // }
        }
    };
}

void
Npc::updatePosition()
{
    if (target->body.isUndetectable)
        return;

    if (Vector2Distance(position, target->position) > detectionRange)
        return;

    position = Vector2MoveTowards(position, target->position, body.speed);
}

void
Npc::draw()
{
    if (destroy)
        return;

    DrawCircle(position.x, position.y, detectionRange, {255, 0, 0, 20});

    Character::draw();
}
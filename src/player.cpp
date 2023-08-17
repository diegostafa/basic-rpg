#include "../include/player.hpp"
#include "../include/collision_detector.hpp"
#include <iostream>
#include <raylib.h>
#include <raymath.h>

Player::Player(AssetManager& assets)
    : Character(Character::Type::PLAYER, assets)
{
    body.speed = 2;
}

std::function<void(const Rectangle&)>
Player::onObstacleCollision(const Vector2& lastPosition)
{
    return [this, lastPosition](const Rectangle&)
    {
        position = lastPosition;
    };
}

std::function<void(Character&)>
Player::onCharacterCollision(const Vector2& lastPosition)
{
    if (body.isUndetectable)
        return nullptr;

    return [this, lastPosition](Character& other)
    {
        if (CheckCollisionPointRec(position, other.getBoundingBox()))
            position = lastPosition;
    };
}

void
Player::updatePosition()
{
    Vector2 direction{0, 0};
    int     acceleration = 0;

    if (IsKeyDown(KEY_UP))
    {
        direction    = {direction.x, direction.y - 1};
        currentState = Character::SpriteState::WALK_UP;
    }
    else if (IsKeyDown(KEY_DOWN))
    {
        direction    = {direction.x, direction.y + 1};
        currentState = Character::SpriteState::WALK_DOWN;
    }
    else if (IsKeyDown(KEY_RIGHT))
    {
        direction    = {direction.x + 1, direction.y};
        currentState = Character::SpriteState::WALK_RIGHT;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        direction    = {direction.x - 1, direction.y};
        currentState = Character::SpriteState::WALK_LEFT;
    }

    if (IsKeyDown(KEY_LEFT_SHIFT) && !(direction.x == 0 && direction.y == 0))
    {
        acceleration = 5;
    }

    sprites.at(currentState).setFrameAnimationSpeed(400 / (body.speed + acceleration));
    position = {position.x + direction.x * (body.speed + acceleration),
                position.y + direction.y * (body.speed + acceleration)};
}
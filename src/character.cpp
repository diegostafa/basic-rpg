#include "../include/character.hpp"
#include <iostream>
#include <raylib.h>
#include <raymath.h>

CharacterId Character::idCounter = 0;

Character::Character(Type type, AssetManager& assets)
    : type(type),
      id(idCounter++),
      currentState(SpriteState::IDLE)
{
    name = typeToName.at(type);

    for (auto&& stateAndSpriteData : characterToSpritesheet.at(type))
    {
        const auto& state      = stateAndSpriteData.first;
        const auto& spriteData = stateAndSpriteData.second;

        sprites[state] = SpriteAnimation(spriteData.first, spriteData.second, assets);
    }
}

Rectangle
Character::getBoundingBox() const
{
    const auto& sprite  = sprites.at(currentState);
    const auto& topLeft = Vector2Subtract(position, {sprite.frameSize.width / 2, sprite.frameSize.height / 2});

    return {
        position.x - (float)sprite.bbox.width / 2,
        position.y - (float)sprite.bbox.height / 2,
        sprite.bbox.width,
        sprite.bbox.height};
}

void
Character::update(CollisionDetector& cd)
{
    const auto lastPosition = position;
    updatePosition();

    auto onObstCollisionCb = onObstacleCollision(lastPosition);
    auto onCharCollisionCb = onCharacterCollision(lastPosition);

    if (onObstCollisionCb)
        cd.registerObstacleCollider(*this, onObstCollisionCb);

    if (onCharCollisionCb)
        cd.registerCharacterCollider(*this, onCharCollisionCb);

    if (body.isUndetectable && body.undetectableTimer.start().update().hasElapsed())
    {
        body.isUndetectable = false;
        body.undetectableTimer.reset();
    }

    sprites.at(currentState).update();
}

void
Character::draw()
{
    const auto& sprite = sprites.at(currentState);

    // draw healthbar
    DrawRectangle(
        position.x - sprite.frameSize.width / 2,
        position.y - sprite.frameSize.height / 2 - 10,
        sprite.frameSize.width,
        5,
        RED);

    DrawRectangle(
        position.x - sprite.frameSize.width / 2,
        position.y - sprite.frameSize.height / 2 - 10,
        sprite.frameSize.width * actualStats.hp / formalStats.hp,
        5,
        GREEN);

    DrawRectangleLines(
        position.x - sprite.frameSize.width / 2,
        position.y - sprite.frameSize.height / 2 - 10,
        sprite.frameSize.width,
        5,
        BLACK);

    sprite.draw(position, body.isUndetectable ? 180 : 255);

    // draw bbox
    // auto bbox = getBoundingBox();
    // DrawRectangleLines(bbox.x, bbox.y, bbox.width, bbox.height, BLUE);
}

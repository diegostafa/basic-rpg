#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "character.hpp"
#include "collision_detector.hpp"
#include <functional>
#include <string>
#include <unordered_map>
#include <utility>

class Player : public Character
{
public:
    Timer attackTimer = Timer(200);

    Player(AssetManager& assets);

    virtual void updatePosition() override;

    virtual std::function<void(const Rectangle&)>
    onObstacleCollision(const Vector2& lastPosition) override;

    virtual std::function<void(Character&)>
    onCharacterCollision(const Vector2& lastPosition) override;
};

#endif
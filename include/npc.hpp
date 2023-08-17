#ifndef NPC_HPP
#define NPC_HPP

#include "character.hpp"
#include "collision_detector.hpp"
#include <unordered_map>

class Npc : public Character
{
private:
    float detectionRange = 100;
    Timer attackTimer    = Timer(200);

public:
    Character const* target;
    bool             destroy;

    Npc(Type type, AssetManager& assets);

    virtual void updatePosition() override;
    virtual void draw() override;

    virtual std::function<void(const Rectangle&)>
    onObstacleCollision(const Vector2& lastPosition) override;

    virtual std::function<void(Character&)>
    onCharacterCollision(const Vector2& lastPosition) override;
};

#endif
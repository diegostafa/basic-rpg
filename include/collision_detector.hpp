#ifndef COLLISION_DETECTOR_HPP
#define COLLISION_DETECTOR_HPP

#include "character.hpp"
#include <functional>
#include <raylib.h>
#include <vector>

class Character;

struct Collider
{
    Character&                            c;
    std::function<void(const Rectangle&)> onObstacleCollision;
    std::function<void(Character&)>       onCharacterCollision;
};

class CollisionDetector
{
private:
    std::vector<Rectangle> m_obstacles;
    std::vector<Collider>  m_obstacleColliders;
    std::vector<Collider>  m_characterColliders;

public:
    void registerObstacles(const std::vector<Rectangle>& colliders);

    void registerObstacleCollider(
        Character&                                   c,
        const std::function<void(const Rectangle&)>& onObstacleCollision);

    void registerCharacterCollider(
        Character&                             c,
        const std::function<void(Character&)>& onCharacterCollision);

    std::vector<Collider>                      detectObstacleCollisions() const;
    std::vector<std::pair<Collider, Collider>> detectCharacterCollisions() const;
};

#endif
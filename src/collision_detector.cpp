#include "../include/collision_detector.hpp"
#include <functional>

void
CollisionDetector::registerObstacles(const std::vector<Rectangle>& obstacles)
{
    m_obstacles.insert(m_obstacles.end(), obstacles.begin(), obstacles.end());
}

void
CollisionDetector::registerObstacleCollider(Character&                                   c,
                                            const std::function<void(const Rectangle&)>& onObstacleCollision)
{
    m_obstacleColliders.push_back({c, onObstacleCollision, nullptr});
}

void
CollisionDetector::registerCharacterCollider(Character&                             c,
                                             const std::function<void(Character&)>& onCharacterCollision)
{
    m_characterColliders.push_back({c, nullptr, onCharacterCollision});
}

std::vector<Collider>
CollisionDetector::detectObstacleCollisions() const
{
    std::vector<Collider> detections;

    for (auto&& collider : m_obstacleColliders)
    {
        for (auto&& obstacle : m_obstacles)
        {
            if (!CheckCollisionRecs(obstacle, collider.c.getBoundingBox()))
                continue;

            if (collider.onObstacleCollision != nullptr)
                collider.onObstacleCollision(obstacle);

            detections.push_back(collider);
        }
    }

    return detections;
}

std::vector<std::pair<Collider, Collider>>
CollisionDetector::detectCharacterCollisions() const
{
    std::vector<std::pair<Collider, Collider>> detections;

    for (int i = 0; i < m_characterColliders.size(); i++)
    {
        for (int j = 0; j < m_characterColliders.size(); j++)
        {
            if (i == j)
                continue;

            if (!CheckCollisionRecs(
                    m_characterColliders[i].c.getBoundingBox(),
                    m_characterColliders[j].c.getBoundingBox()))
                continue;

            if (m_characterColliders[i].onCharacterCollision != nullptr)
                m_characterColliders[i].onCharacterCollision(m_characterColliders[j].c);

            if (m_characterColliders[j].onCharacterCollision != nullptr)
                m_characterColliders[j].onCharacterCollision(m_characterColliders[i].c);

            detections.push_back({m_characterColliders[i], m_characterColliders[j]});
        }
    }

    return detections;
}

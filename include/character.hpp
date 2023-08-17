#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "collision_detector.hpp"
#include "drawable.hpp"
#include "sprite_animation.hpp"
#include <map>
#include <memory>
#include <raylib.h>
#include <string>

class CollisionDetector;

typedef unsigned int CharacterId;

class Character : public Drawable
{
private:
    static CharacterId idCounter;

public:
    enum class SpriteState
    {
        IDLE,
        WALK_UP,
        WALK_DOWN,
        WALK_LEFT,
        WALK_RIGHT
    };

    enum class Type
    {
        PLAYER,
        BIRDO,
        DJINN_VENUS
    };

    struct Stats
    {
        int hp  = 100;
        int mp  = 10;
        int atk = 10;
        int def = 10;
        int spd = 10;
    };

    struct Body2d
    {
        float speed             = 0;
        bool  isUndetectable    = false;
        Timer undetectableTimer = Timer(5000);
    };

    const CharacterId id;
    const Type        type;
    std::string       name        = "n/a";
    Stats             formalStats = {};
    Stats             actualStats = {};
    Body2d            body        = {};

    std::unordered_map<SpriteState, SpriteAnimation> sprites;
    SpriteState                                      currentState;

    Character(Type type, AssetManager& assets);

    void         update(CollisionDetector& cd);
    Rectangle    getBoundingBox() const;
    virtual void updatePosition() = 0;
    virtual void draw() override;

    virtual std::function<void(const Rectangle&)>
    onObstacleCollision(const Vector2& lastPosition) = 0;

    virtual std::function<void(Character&)>
    onCharacterCollision(const Vector2& lastPosition) = 0;

    const std::unordered_map<Type, std::string> typeToName = {
        {Type::PLAYER, "Player"},
        {Type::BIRDO, "Birdo"},
    };

    const std::unordered_map<Type, std::unordered_map<SpriteState, std::pair<std::string, std::string>>>
        characterToSpritesheet = {
            {Type::PLAYER,
             {
                 {SpriteState::IDLE, {"assets/sprites/player/idle.png", "assets/sprites/player/idle.json"}},
                 {SpriteState::WALK_DOWN, {"assets/sprites/player/down.png", "assets/sprites/player/down.json"}},
                 {SpriteState::WALK_LEFT, {"assets/sprites/player/left.png", "assets/sprites/player/left.json"}},
                 {SpriteState::WALK_RIGHT, {"assets/sprites/player/right.png", "assets/sprites/player/right.json"}},
                 {SpriteState::WALK_UP, {"assets/sprites/player/up.png", "assets/sprites/player/up.json"}},
             }},

            {Type::BIRDO,
             {
                 {SpriteState::IDLE, {"assets/sprites/monsters/birdo.png", "assets/sprites/monsters/birdo.json"}},
             }},
        };
};

#endif
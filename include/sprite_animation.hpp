#ifndef SPRITE_ANIMATION_HPP
#define SPRITE_ANIMATION_HPP

#include "../deps/ray-widgets/helpers/include/timer.hpp"
#include "asset_manager.hpp"
#include <nlohmann/json.hpp>
#include <raylib.h>
#include <string>

class SpriteAnimation
{
private:
    Texture2D      m_spriteSheet;
    nlohmann::json m_frameData;
    int            m_totalFrames;
    int            m_currentFrame;
    Timer          m_frameTimer = Timer(300);

    void incrementFrameCounter();
    void setCurrentSpriteFrame();

public:
    Rectangle frameSize;
    Rectangle bbox;

    SpriteAnimation() = default;
    SpriteAnimation(const std::string& imageFile, const std::string& dataFile, AssetManager& assets);

    void setFrameAnimationSpeed(int milliseconds);
    void update();
    void draw(const Vector2& position, unsigned char alpha) const;
};

#endif